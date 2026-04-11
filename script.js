import { AFND_DATA } from './afndData.js';
import { analisarSentenca } from './lexer.js';
import { UI } from './ui.js';

/**
 * CONTROLLER
 * Coordena a interação entre a Lógica (Lexer) e a Interface (UI).
 */

window.executarAnalise = function() {
    const tokenType = UI.getElement('tokenType').value;
    const input = UI.getElement('input').value || '';

    if (!input) {
        alert('Digite uma entrada!');
        return;
    }

    const afnd = AFND_DATA[tokenType];
    UI.mostrarDiagrama(afnd);
    
    // Executa o lexer
    const analise = analisarSentenca(input, AFND_DATA);
    
    // Atualiza a interface
    UI.limparTabelas();
    UI.atualizarTabelaSimbolos(analise.tabelaSimbolos);
    UI.atualizarFita(analise.fita);
    
    // Mostra detalhes do primeiro token (para animação/diagrama)
    if (analise.resultados.length > 0) {
        const primeiro = analise.resultados[0];
        UI.mostrarTransicoes(primeiro.transicoes);
        UI.mostrarResultado(primeiro);
    }
}

window.limpar = function() {
    UI.limparGeral();
}

window.mostrarExemplo = function() {
    const btn = UI.getElement('exampleButtons');
    btn.style.display = btn.style.display === 'none' ? 'grid' : 'none';
}

window.carregarExemplo = function(exemplo) {
    UI.getElement('input').value = exemplo;
    window.executarAnalise();
}

// Executar exemplo ao carregar
window.addEventListener('load', () => {
    // Inicialização opcional
});
