from solution import Value

class Account:
    amount = Value()

    def __init__(self, commission):
        self.commission = commission

if __name__ == '__main__':
    new_account = Account(0.1)
    new_account.amount = 100
    print(new_account.amount)
