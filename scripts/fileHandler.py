def fileReader(filepath) : 
    f=open(filepath,'rb')

    print('File Header Bytes: ')
    steps = {2,4,4,4}
    for num in steps:
        print(str(f.read(num)) + " ")
    print(/n)

    i = 0
    steps = {4,4,4,2,2,4,4,4,4,4,4}
    for num in steps:
        if i = 1:
            print(/n+'Image Width: ' + str(f.read(num)) + /n)
        elif i = 2:
            print(/n+'Image Height: ' + str(f.read(num)) + /n)
        else:
            print(str(f.read(num)) + " ")
            
        i+=1
        #i++??
    print(/n)
