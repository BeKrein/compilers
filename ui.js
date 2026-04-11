/**
 * INTERFACE HANDLING LOGIC
 * Funções responsáveis por atualizar o DOM, tabelas, diagramas e animações.
 */

export const UI = {
    getElement: (id) => document.getElementById(id),

    mostrarDiagrama: (afnd) => {
        const estados = afnd.estados.map(e => {
            const isFinal = afnd.finais.includes(e);
            const isInicial = e === afnd.inicial;
            let marcacao = '';
            if (isInicial && isFinal) marcacao = ' (inicial, final)';
            else if (isInicial) marcacao = ' (inicial)';
            else if (isFinal) marcacao = ' (final)';
            return e + marcacao;
        });

        UI.getElement('stateDiagram').innerHTML = 
            '<strong>Estados:</strong> ' + estados.join(' → ');
    },

    mostrarTransicoes: (transicoes) => {
        const html = transicoes.map(t => `
            <div class="transition-item ${t.class}">
                Passo ${t.passo}: ${t.estado} --${t.simbolo}--> ${t.proximo}
            </div>
        `).join('');

        UI.getElement('transitionsList').innerHTML = html || 'Nenhuma transição registrada';
    },

    mostrarResultado: (resultado) => {
        const resultBox = UI.getElement('resultBox');
        resultBox.style.display = 'block';
        resultBox.className = 'result-box ' + (resultado.sucesso ? 'success' : 'error');
        
        UI.getElement('resultStatus').textContent = resultado.sucesso ? '✓ Aceito' : '✗ Rejeitado';
        UI.getElement('resultToken').textContent = resultado.lexema;
        UI.getElement('resultType').innerHTML = `<span class="badge badge-${resultado.tipo}">${resultado.tipo.replace('_', ' ')}</span>`;
        UI.getElement('resultSteps').textContent = resultado.transicoes.length;
    },

    limparTabelas: () => {
        UI.getElement('tableBody').innerHTML = '<tr><td colspan="4" style="text-align: center; color: #999;">Analisando...</td></tr>';
        UI.getElement('tokenList').innerHTML = '';
    },

    atualizarTabelaSimbolos: (tabela) => {
        const tbody = UI.getElement('tableBody');
        if (tabela.length === 0) {
            tbody.innerHTML = '<tr><td colspan="4" style="text-align: center; color: #999;">Nenhum token reconhecido</td></tr>';
            return;
        }

        tbody.innerHTML = tabela.map(item => `
            <tr>
                <td>${item.linha}</td>
                <td>${item.identificador}</td>
                <td>${item.rotulo}</td>
                <td>${item.idFita}</td>
            </tr>
        `).join('');
    },

    atualizarFita: (fitaStr) => {
        const tokenList = UI.getElement('tokenList');
        tokenList.innerHTML = `<div class="fita-display">${fitaStr}</div>`;
    },

    limparGeral: () => {
        UI.getElement('input').value = '';
        UI.getElement('resultBox').style.display = 'none';
        UI.getElement('tableBody').innerHTML = '<tr><td colspan="4" style="text-align: center; color: #999;">Nenhum token reconhecido ainda</td></tr>';
        UI.getElement('tokenList').innerHTML = '<span style="color: #999;">E1 E2 E3 ... $</span>';
        UI.getElement('transitionsList').innerHTML = 'Selecione uma entrada para ver as transições';
        UI.getElement('stateDiagram').innerHTML = 'q0 (inicial) → estados específicos';
    }
};
