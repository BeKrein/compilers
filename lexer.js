/**
 * CORE LEXER LOGIC
 * Requisitos do Trabalho: Implementação do algoritmo de reconhecimento,
 * geração da FITA de saída e Tabela de Símbolos.
 */

export function analisarSentenca(texto, afndData) {
    const tokens = texto.split(/\s+/).filter(t => t.length > 0);
    const resultados = [];
    const fita = [];
    const tabelaSimbolos = [];
    let linhaAtual = 1;

    // Se o texto contém novas linhas, podemos dividir por linhas primeiro para contar
    const linhas = texto.split('\n');
    let fitaCounter = 1;

    linhas.forEach((linhaStr, index) => {
        const linhaNum = index + 1;
        const lexemas = linhaStr.trim().split(/\s+/).filter(l => l.length > 0);

        lexemas.forEach(lexema => {
            const resultado = reconhecerLexema(lexema, afndData);
            
            if (resultado.sucesso) {
                const idFita = `E${fitaCounter++}`;
                fita.push(idFita);
                
                tabelaSimbolos.push({
                    linha: linhaNum,
                    identificador: lexema,
                    rotulo: resultado.tipo.toUpperCase(),
                    idFita: idFita
                });
            } else {
                fita.push('ERR');
            }
            
            resultados.push({
                lexema,
                ...resultado
            });
        });
    });

    return {
        resultados,
        fita: fita.join(' ') + ' $',
        tabelaSimbolos
    };
}

function reconhecerLexema(lexema, afndData) {
    // Tenta reconhecer o lexema em cada tipo de autômato
    // No trabalho real, muitas vezes há um AFD único ou uma ordem de prioridade
    const tipos = Object.keys(afndData);
    
    for (const tipo of tipos) {
        const afnd = afndData[tipo];
        const processamento = processarAFD(lexema, afnd);
        
        if (processamento.sucesso) {
            return {
                sucesso: true,
                tipo: tipo,
                transicoes: processamento.transicoes,
                estadoFinal: processamento.estadoFinal
            };
        }
    }

    // Se nenhum reconheceu, retorna erro (usando o primeiro autômato para mostrar o caminho do erro se necessário, ou uma falha genérica)
    return {
        sucesso: false,
        tipo: 'DESCONHECIDO',
        transicoes: [], // Poderíamos preencher com o caminho de erro do autômato mais próximo
        estadoFinal: 'qe'
    };
}

function processarAFD(input, afnd) {
    const transicoes = [];
    let estadoAtual = afnd.inicial;
    let passo = 0;
    const inputComSeparador = input + '@';

    for (let i = 0; i < inputComSeparador.length; i++) {
        const char = inputComSeparador[i];
        // Encontra transição. Aqui simplificamos a busca de símbolos (i,w,f,r,v ou a-z ou 0-9)
        const transicao = afnd.transicoes.find(t => {
            if (t.from !== estadoAtual) return false;
            if (t.symbol === char) return true;
            if (t.symbol === 'a-z' && /^[a-z]$/.test(char)) return true;
            if (t.symbol === 'a-z,A-Z,_' && /^[a-zA-Z_]$/.test(char)) return true;
            if (t.symbol === 'a-z,A-Z,0-9,_' && /^[a-zA-Z0-9_]$/.test(char)) return true;
            if (t.symbol === '0-9' && /^[0-9]$/.test(char)) return true;
            if (t.symbol === 'i,w,f,r,v' && /^[iwfrv]$/.test(char)) return true;
            if (t.symbol === '[^"]' && char !== '"' && char !== '@') return true;
            return false;
        });
        
        if (transicao) {
            transicoes.push({
                passo: passo++,
                estado: estadoAtual,
                simbolo: char,
                proximo: transicao.to,
                class: afnd.finais.includes(transicao.to) ? 'accepted' : ''
            });
            estadoAtual = transicao.to;
        } else {
            return { sucesso: false, transicoes: [], estadoFinal: 'qe' };
        }
    }

    const sucesso = afnd.finais.includes(estadoAtual);
    return { sucesso, transicoes, estadoFinal: estadoAtual };
}
