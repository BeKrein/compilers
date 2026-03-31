def main():
    rules = readRegularGrammar()
    sourceCode = readSourceCode()
    transactions = createDictionary(rules)
    print(transactions)

def getReservedWords() -> set[str]:
    return { "se", "sai", "foi" }

def readRegularGrammar() -> list[str]:
    with open("grammar.txt", 'r', encoding='utf-8') as f:
        rules = f.readlines()
    return rules

def readSourceCode() -> list[str]:
    with open("entrada.txt", 'r', encoding='utf-8') as f:
        sourceCode = f.read().split()
    return sourceCode

def readReservedWords() -> set[str]:
    with open("entrada.txt", 'r', encoding='utf-8') as f:
        reservedWords = f.read().split()
    return set(reservedWords)

def createDictionary(rules: list[str]) -> dict[str, list[str]]:
    transactions = {}
    for line in rules:
        left, right = line.split("::=")
        left = left.strip()
        right = right.strip()
        transactions[left] = [op.strip() for op in right.split("|")]
    return transactions

if __name__ == "__main__":
    main()