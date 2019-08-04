def sum_bits(a, b, c):
    s1 = a^b
    result = s1^c

    c1 = s1 & c
    c2 = a & b

    c_out = c1 | c2

    return (result, c_out)


def sum(a, b):
    c = 0
    result = 0

    n_bits = max(len(bin(a)[2:]), len(bin(b)[2:]))

    for i in range(n_bits):
        result = result
        aux = sum_bits((a >> i) & 1, (b >> i) & 1, c)

        c = aux[1]
        result += (aux[0] << i)

    return result


def old_mult(a, b):

    n_bits = max(len(bin(a)[2:]), len(bin(b)[2:]))
    res = 0
    aux = 0
    for i in range(2**n_bits):
        tmps = (b == aux)

        factor = 0 if tmps else a
        sumando = 0 if tmps else 1

        aux = aux + sumando
        res = res + factor

    return res


def mult(a, b):
    n_bits = max(len(bin(a)[2:]), len(bin(b)[2:]))
    res = 0
    for i in range(n_bits):
        aux_a = (a>>i & 0b1)
        aux_b = 0
        for j in range(n_bits):
            aux_b += ((b >> j & 0b1) & aux_a) << j
        res += aux_b << i
    return res

print(sum(4, 10))
print(mult(4, 2))
