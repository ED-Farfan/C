import os
import time

N=[2,4,8,16,32]
t=[]
for i in N:
    print("----------------------------------------------------")
    inicio = time.time()
    print("             Hilos: ",i,"\n")
    os.system("./hilos 2000 1500 1500 20000 "+str(i))
    fin = time.time()
    tiempo =fin-inicio
    t.append(tiempo)
    print("\nTeimpo de ejecuccion:"+str(tiempo)+"\n\n")    
    print("----------------------------------------------------")    
for i in t:
    print(i)
f = open ("fichero.txt", "w")
texto="Hilos\tTiempo\n"
for i in range(0,len(t)):
    texto += str(N[i])+"\t"+str(tiempo[i])+"\n"
f.write(texto)
f.close()
