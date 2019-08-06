
def divide(a, b):

    bits_a = len(bin(a)) - 2
    bits_b = len(bin(a)) - 2

    if a == b:
        return 1, 0

    mayor = max(a, b)
    if mayor == b:
        return 0, a

    for i in range(bits_a):
        aux = (a >> (bits_a - i - 1))

        if aux < b:
            continue

        rem = aux - b

        next_a = a - (aux << (bits_a - i - 1)) + (rem << (bits_a - i - 1))

        n, rem = divide(next_a, b)

        bits_n = len(bin(n)) - 2

        return ((1 << (bits_a - i - 1)) + n), rem

for i in range(10)[1:]:
    for j in range(i)[1:]:
        print(i, j, divide(i, j))
