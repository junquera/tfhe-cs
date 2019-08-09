mediaY = 0
mediaX = 0
covarianza = 0
varianzaX = 0
fi = 1

def _varianza(xs):
    varianza = 0
    media = 0

    for x in xs:
        varianza += (x**2 * fi)
        media += x

    media = float(media)/len(xs)

    varianza = (float(varianza)/len(xs)) - media**2

    return varianza


def _covarianza(xys):
    mediaX = 0
    mediaY = 0
    covarianza = 0
    for x,y in xys:
        covarianza += x*y*fi
        mediaX += x
        mediaY += y

    mediaX = float(mediaX)/len(xys)
    mediaY = float(mediaY)/len(xys)

    covarianza = (float(covarianza)/len(xys)) - (mediaX * mediaY)

    return covarianza

def regresion(xys):
    mediaX = 0
    mediaY = 0

    for x,y in xys:
        mediaX += x
        mediaY += y

    mediaX = float(mediaX)/len(xys)
    mediaY = float(mediaY)/len(xys)

    covarianza = _covarianza(xys)
    varianza = _varianza([x[0] for x in xys])

    m = covarianza/varianza
    n = mediaY - (m * mediaX)

    return (m, n)


xs = [5, 7, 6, 3, 8]
xys = [(5,6), (7,9), (6,6), (3,4), (8,7)]
print(_varianza(xs))
print(_covarianza(xys))
print(regresion(xys))
