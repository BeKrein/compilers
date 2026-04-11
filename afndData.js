export const AFND_DATA = {
    palavra_chave: {
        estados: ['q0', 'q1', 'q2', 'q3'],
        inicial: 'q0',
        finais: ['q3'],
        transicoes: [
            { from: 'q0', symbol: 'i,w,f,r,v', to: 'q1' },
            { from: 'q1', symbol: 'a-z', to: 'q2' },
            { from: 'q2', symbol: 'a-z', to: 'q2' },
            { from: 'q2', symbol: '@', to: 'q3' }
        ],
        exemplos: ['if', 'while', 'for', 'return', 'void']
    },
    identificador: {
        estados: ['q0', 'q9', 'qf'],
        inicial: 'q0',
        finais: ['q9'],
        transicoes: [
            { from: 'q0', symbol: 'a-z,A-Z,_', to: 'q9' },
            { from: 'q9', symbol: 'a-z,A-Z,0-9,_', to: 'q9' },
            { from: 'q9', symbol: '@', to: 'qf' }
        ],
        exemplos: ['x', 'myVar', '_count', 'ABC']
    },
    numero_int: {
        estados: ['q0', 'q16', 'qf'],
        inicial: 'q0',
        finais: ['q16'],
        transicoes: [
            { from: 'q0', symbol: '0-9', to: 'q16' },
            { from: 'q16', symbol: '0-9', to: 'q16' },
            { from: 'q16', symbol: '@', to: 'qf' }
        ],
        exemplos: ['0', '42', '1234', '999']
    },
    numero_float: {
        estados: ['q0', 'q16', 'q20', 'q21', 'qf'],
        inicial: 'q0',
        finais: ['q21'],
        transicoes: [
            { from: 'q0', symbol: '0-9', to: 'q16' },
            { from: 'q16', symbol: '0-9', to: 'q16' },
            { from: 'q16', symbol: '.', to: 'q20' },
            { from: 'q20', symbol: '0-9', to: 'q21' },
            { from: 'q21', symbol: '0-9', to: 'q21' },
            { from: 'q21', symbol: '@', to: 'qf' }
        ],
        exemplos: ['3.14', '0.5', '99.99']
    },
    string: {
        estados: ['q0', 'q23', 'q24', 'qf'],
        inicial: 'q0',
        finais: ['q24'],
        transicoes: [
            { from: 'q0', symbol: '"', to: 'q23' },
            { from: 'q23', symbol: '[^"]', to: 'q23' },
            { from: 'q23', symbol: '"', to: 'q24' },
            { from: 'q24', symbol: '@', to: 'qf' }
        ],
        exemplos: ['"hello"', '"test"', '""']
    },
    operador: {
        estados: ['q0', 'q26', 'q27', 'q31', 'q32', 'qf'],
        inicial: 'q0',
        finais: ['q27', 'q31', 'q32'],
        transicoes: [
            { from: 'q0', symbol: '=', to: 'q26' },
            { from: 'q26', symbol: '=', to: 'q27' },
            { from: 'q0', symbol: '+', to: 'q31' },
            { from: 'q0', symbol: '-', to: 'q32' },
            { from: 'q27', symbol: '@', to: 'qf' },
            { from: 'q31', symbol: '@', to: 'qf' }
        ],
        exemplos: ['=', '==', '+', '-', '*']
    }
};