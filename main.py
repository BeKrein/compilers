initialState = 0

reservedWords = { "se", "sai", "foi" }

def main():
    rules = readRegularGrammar()
    transactions = createDictionary(rules)
    print(transactions)

def readRegularGrammar() -> list[str]:
    with open("gr.txt", 'r', encoding='utf-8') as f:
        rules = f.readlines()
    return rules

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