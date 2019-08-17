import json

for i in range(2018 - 2012 + 1):
    year = 2012 + i
    with open('finisterre%d.json' % year) as f:
        j = json.loads(f.read())

    for d in j:
        mes = int(d.get('fecha').split('-')[1])
        temperatura = float(d.get('tm_mes'))
        print("%s,%s,%s" % (year, mes, temperatura))
