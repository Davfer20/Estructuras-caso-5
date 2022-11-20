
def decoder(pString):
    originalString = pString
    LowerCaseS = originalString.lower()
    SplitS = LowerCaseS.split(" ")

    SplitS2 = []
    for n in SplitS:
        if len(n) >= 4:
            SplitS2.append(n)

    halfWords = []
    for n in SplitS2:
        halfWords.append(n[:4])
    
    return halfWords



    
print(decoder("Ocupo una institución educativa para mis hijos, quienes poseen gran talento para las áreas STEM, y quienes desean ser futuros ingenieros"))