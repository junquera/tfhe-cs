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


print(sum(4, 10))
