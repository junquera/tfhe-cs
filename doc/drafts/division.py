
def shiftr(a, n):
    res = []
    for i in range(16):
        pos = i - n
        res.append(0 if pos < 0 else a[pos])
    return res

def shiftl(a, n):
    res = []
    for i in range(16):
        pos = n + i
        res.append(0 if pos >= 16 else a[n + i])
    return res

def add(a, b):
    res = []
    c = 0
    for i in range(16):
        s1 = a[15-i] ^ b[15-i]
        r = s1 ^ c

        c1 = s1 & c
        c2 = a[15-i] & b[15-i]
        c = c1 | c2
        res.append(r)

    return res[::-1]

bits = 16
cero = [0 for _ in range(bits)]
uno = [0 for _ in range(bits)]
uno[-1] = 1

debug = False

def neg(a):
    res = []
    for i in a:
        res.append(0 if i else 1)

    res = add(res, uno)
    return res

def sub(a, b):
    return add(a, neg(b))

def mayor_igual(a, b):
    return bits2int(a) >= bits2int(b)

def divide(dividendo, divisor):

    cociente = [i for i in cero]
    resto = [i for i in cero]

    padding = int(bits/2)


    divisor = shiftl(divisor, padding - 1)

    for i in range(padding):
        if debug:
            print(i)
            print(dividendo)
            print(divisor)
            print(cociente)
            print(resto)
            input()
        gt = 1 if mayor_igual(dividendo, divisor) else 0
        cociente[padding + i] = gt

        resto = sub(dividendo, divisor) if gt else resto
        dividendo = resto if gt else dividendo

        divisor = shiftr(divisor, 1)

    return cociente


def int2bits(a):
    res = []
    for i in range(bits):
        res.append(a>>(15-i)&0b1)
    return res

def bits2int(a):
    res = 0
    for i in range(bits):
        res += (2**(15-i))*a[i]
    return res

def test(a, b):
    bits_a = []
    bits_b = []

    for i in range(16):
        bits_a.append(a>>(15-i)&0b1)
        bits_b.append(b>>(15-i)&0b1)

    res = divide(bits_a, bits_b)

    print(a, '/', b, '=', bits2int(res), bits2int(res)==int(a/b))


# debug = False
#
# for b in range(20)[2:]:
#     for a in range(20):
#         test(a, b)

debug = True
test(82, 30)
