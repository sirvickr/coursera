from classes import A, B, C


if __name__ == "__main__":
    data = [0.1, 0.4, 1]
    result = [0.1, 0.3, 1]
    a = A(data, result)
    b = B(data, result)
    c = C(data, result)
    print(f"\nanswer = {a.get_answer()} B: pre {b.get_pre()} rec {b.get_rec()}")

    print(f"A: score = {a.get_score()} loss = {a.get_loss()}")
    # print(f"B: score = {b.get_score()} loss = {b.get_loss()}")
    print(f"B: score = {b.get_score()}")
    print(f"C: score = {c.get_score()} loss = {c.get_loss()}")
