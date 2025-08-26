import sage



def printInFileMatrix(filename,mat):

    f=open(filename,"w")

    f.write(mat.str())

    f.flush()

    f.close()



def printInFile(filename,message):

    f=open(filename,"w")

    f.write(message)

    f.flush()

    f.close()





def pickDistinctInRange(low,high,n):

    L=[]

    i=0

    while i<n:

        r=randint(low,high)

        if r in L:

            continue

        else:

            L.append(r) 

            i+=1

    return L



def randomSparseVector(n,maxWeight):

    V=[0 for k in range(n)]

    w=randint(1,maxWeight)

    I=pickDistinctInRange(0,n-1,w)

    for i in I:

        V[i]=1

    return V



def cyclicRightShift(v,shifting):

    n=len(v)

    shift=shifting%n

    shifted=copy(v)

    for i in range(n):

        shifted[(i+shift)%n]=v[i]



    return shifted





def generate_QC_LDPC_code(n,circulantShape,maxWeight,field=GF(2)):

    rows,cols=circulantShape

    if rows>cols:

        print("wrong circulant block size rows must be at most equal to the numer of columns")

    

    V=[vector(GF(2),randomSparseVector(n,maxWeight)) for k in range(rows)]

    

    M=[]+V

    

    for i in range(1,n//cols):

        L=[len(M)-l for l in range(rows,0,-1)] 

        for t in L:

            M.append(cyclicRightShift(M[t],cols))



    mat=matrix(field,M)



    dualC=codes.LinearCode(mat)

    

    genC=mat.right_kernel().basis()



    matC=matrix(field,genC)



    C=codes.LinearCode(matC)   

    

    

    return C,V



def generate_QC_LDPC_code_from_sparse_vector(v,blocSize,field):

    V=copy(v)

    n=len(v)

    M=[V]

    

    for i in range(n//blocSize):

        M.append(cyclicRightShift(M[-1],blocSize))

    mat=matrix(field,M)

    dualC=codes.LinearCode(mat)

    

    genC=dualC.generator_matrix().right_kernel().basis()



    matC=matrix(field,genC)

    C=codes.LinearCode(matC)   

    

    

    return C



def sample(s,low,high):

    i=0

    L=[]

    while i<s:

        r=randint(low,high)

        if r in L:

            continue

        else:

            L.append(r)

            i+=1

    return L





def sample_matrix(C,ms,ns):

    l=C.nrows()

    c=C.ncols()

    lines=sample(ms,0,l-1)

    colums=sample(ns,0,c-1)

    lines.sort()

    colums.sort()

    return C[lines,colums],lines,colums



def HW(v): #hamming weight

    w=0

    for i in v.list():

        if i!=0:

            w+=1



    return w



def pickDistinctInRange(low,high,n):

    L=[]

    i=0

    while i<n:

        r=randint(low,high)

        if r in L:

            continue

        else:

            L.append(r) 

            i+=1



    return L



def VectorSwapCoordinates(v,i,j):

    V=v.list()

    temp=V[i]

    V[i]=v[j]

    V[j]=temp

    return vector(V)



def swapColums(M,i,j):

    M_l=[]

    for i in range(M.nrows()):

        M_l.append(M[i])

    

    swapM_l=[]

    for v in M_l:

        swapM_l.append(VectorSwapCoordinates(v,i,j))

    

    return matrix(swapM_l)



def genRandomBinaryVector(size):

    L=[]

    for i in range(size):

        r=random()

        if r<0.5:

            L.append(1)

        else:

            L.append(0)

    return vector(L)







def genErrorVector(n,p):

    L=[]

    

    for i in range(n):

        r=random()

        if r<p:

            L.append(1)

        else:

            L.append(0)

    return vector(L)





def getSomeNoisyCodewords(C,n_codewords,noise):

    n=C.dimension()

    numberOfErrors=0

    L=[]

    for i in range(n_codewords):

        m=genRandomBinaryVector(n)

        c=C.encode(m) 

        e=genErrorVector(C.length(),noise)

        numberOfErrors+=HW(e)

        L.append(c+e)

    return L,numberOfErrors



def leftEcholonizeMatrix(H):

    H_c=copy(H)

    blocksize=H_c.nrows() 

    A=H_c[:,blocksize:H_c.ncols()]

    Id=H_c[:,0:blocksize]

    H_c=block_matrix(GF(2),[[A,Id]])

    H_c.echelonize()

    Id=H_c[:,0:blocksize]

    A=H_c[:,blocksize:H_c.ncols()]

    H_c=block_matrix(GF(2),[[A,Id]])



    return H_c



def getOrbit(v,blockSize):

    N=len(v)

    card=N//blockSize

    L=[v]

    for i in range(card-1):

        L.append(cyclicRightShift(L[-1],blockSize))

    return L



def composePermutation(p1,p2): #retourne p1 rond p2

    L=[]

    n=len(p2)

    for i in range(0,n):

        L.append(p1[p2[i]])

    return L



def transpostion(i,j,n):

    L=[i for i in range(n)]

    L[i]=j

    L[j]=i

    return L



def columnIsZero(M,j):

    n=M.nrows()

    for i in range(n):

        if M[i,j] != 0:

            return False

    return True



def swapCols(M,i,j): #modifie l'entrée

    n=M.nrows()

    v=[M[k,i] for k in range(n)]

    for l in range(n):

        M[l,i]=M[l,j]

        M[l,j]=v[l]



def LinCombinationRows(M,i,j): #ligne i vaut elle même plus la ligne j

    n = M.ncols()

    for k in range(n):

        M[i,k]+=M[j,k]



def swapRows(M,i,j):

    v=list(M[i])

    n=len(v)

    for k in range(n):

        M[i,k]=M[j,k]

        M[j,k]=v[k]



def findLinePivot(M,col):

    n=M.nrows()

    for i in range(col,n):

        if M[i,col]!=0:

            return i



    return -1



def findNonZeroUnder(M,col,index): #trouve un coefficient non nul en dessous d'une certaine ligne a colonne fixee

    n=M.nrows()

    for i in range(index,n):

        if M[i,col]!=0:

            return i

    return -1







def applyPermutationOnVector(v,sigma):

    L=[]

    n=len(v)

    for i in range(n):

        L.append(v[sigma[i]])



    return vector(L)



def inversePermutation(sigma):

    n=len(sigma)

    L=[i for i in range(n)]

    for i in range(n):

        L[sigma[i]]=i

    return L

    





def reduce(M): 

    n=M.ncols()

    m=M.nrows()

    colsPermutation=[i for i in range(n)]

    E=copy(M)

    for i in range(m):

        if columnIsZero(E,i):

            found=False

            for k in range(i+1,n):

                if not columnIsZero(E,k):

                    swapCols(E,k,i)

                    t=transpostion(i,k,n)

                    colsPermutation=composePermutation(t,colsPermutation)

                    found=True

                    break

            if not found:

                return "impossible de continuer la reduction car plus aucune colonne non nulle n'est disponible"

        

        pivotRow=findLinePivot(E,i)

        if pivotRow==-1:

            found=False

            for k in range(i+1,n):

                pivotRow=findNonZeroUnder(E,k,i)

                if pivotRow!=-1:

                    swapCols(E,k,i)

                    t=transpostion(i,k,n)

                    colsPermutation=composePermutation(t,colsPermutation)

                    break

            if not found:

                print("colonne avec un bon pivot introuvable")



        if pivotRow!=i:

            swapRows(E,i,pivotRow) 



        for k in range(i+1,m): #on annule tout les coeff sous le pivot

            if E[k,i]!=0:

                LinCombinationRows(E,k,i)

    



    for i in range(m-1,-1,-1):

        for k in range(i):

            if E[k,i]!=0:

                LinCombinationRows(E,k,i)

        

        

                

    

    return E,colsPermutation







def left_reduce(M):

    n=M.ncols()

    m=M.nrows()

    colsPermutation=[i for i in range(n)] 

    E=copy(M)

    for i in range(m):

        if columnIsZero(E,n-m+i):

            found=False

            for k in range(0,n-m+i):

                if not columnIsZero(E,n-m+k):

                    swapCols(E,k,n-m+i)

                    t=transpostion(n-m+i,k,n)

                    colsPermutation=composePermutation(t,colsPermutation)

                    found=True

                    break

            if not found:

                return "impossible de continuer la reduction car plus aucune colonne non nulle n'est disponible"

        

        pivotRow=findNonZeroUnder(E,n-m+i,i)

        if pivotRow==-1:

            found=False

            for k in range(n-m+i+1,n-m+i+n):

                pivotRow=findNonZeroUnder(E,k%n,i)

                if pivotRow!=-1:

                    found=True

                    swapCols(E,k%n,n-m+i)

                    t=transpostion(n-m+i,k%n,n)

                    colsPermutation=composePermutation(t,colsPermutation)

                    break

            if not found:

                print("colonne avec un bon pivot introuvable")

            



        if pivotRow!=i:

            swapRows(E,i,pivotRow)



        for k in range(i+1,m):

            if E[k,n-m+i]!=0:

                LinCombinationRows(E,k,i)



    for i in range(0,m): 

        for k in range(i-1,-1,-1):

            if E[k,n-m+i]!=0:

                LinCombinationRows(E,k,i)

        

    return E,colsPermutation

        

def proj(v,indices): #on ne garde que les coordonée dans indices 

    indices.sort()

    V=[]

    for i in indices:

        V.append(v[i])

    return vector(V)



def extend(v,n,indices): #on complete avec des 0 en conservant les coordonee dans indice à la bonne place

    V=[0 for i in range(n)]

    N=len(indices)

    for i in range(N):

        V[indices[i]]=v[i]

    return vector(V)



    

def SVR(H_b,ms,ns,permutation,d_s,w_thres,N_iter,field): #fonctionnne quand il n'y a pas d'erreur dans les mots de codes reçus

    H_r=[]

    perm=permutation

                                      #et que le code admet une matrice generatrice sous forme systematique

    H_c=copy(H_b)

    for i in range(N_iter): 

        inv_perm=inversePermutation(perm) 



        #X=[]

        #d=genRandomBinaryVector(ns)

        #L=pickDistinctInRange(0,ms-1,d_s)

        #for k in range(ns-ms):

            #eq=True

            #for j in L:

                #if H_c[k,j]==d[j]:

                    #continue

                #else:

                    #eq=False

                    #break

            #if eq:

                #X.append(H_c[k])

        #lenX=len(X)



        

        for u in range(len(list(H_c))):#for u in range(lenX):

            for v in range(u,len(list(H_c))):#for v in range(u,lenX)

                s=H_c[u]+H_c[v] #s=X[u]+X[v]

                if HW(s)+2<=w_thres and H_c[u]!=H_c[v]: #if HW(s)+2<=w_thres and X[u]!=X[v]:

                    H_r.append(applyPermutationOnVector(s,inv_perm))



        j=randint(0,ms-1)

        k=randint(ms,ns-1)

        H_c=swapColums(H_c,j,k)



        H_c,perm=left_reduce(H_c)



    return H_r



def AllInDual(C,L):

    M=C.systematic_generator_matrix()

    w=0

    g=True

    for v in L:

        if not(v in C.dual_code()):

            w+=1

            g=False



    print("number of words not in dual:",w,"total number of words:",len(L))

    print("ratio of wrong parity check words:",float(w/len(L)))

    return g







def sampleLinearlyIndependent(rec,sampleSize):

    N=len(rec)

    s=0

    L=[]

    d=0

    

    mat_rec=Matrix(GF(2),rec)

    Lim=codes.LinearCode(mat_rec)

    while s<min([sampleSize,Lim.dimension()]):

        ind=randint(0,N-1)

        mat=Matrix(GF(2),L+[rec[ind]])

        C=codes.LinearCode(mat)

        if C.dimension()>d:

            L.append(rec[ind])

            d+=1

            s+=1

    

    return L







    





    return C,rec,L,numErrors





def trySVR_fixedCode(C,blocksize,w_thres,numberReceivedWords,errorRate,ms,ns):

    rec,numErrors=getSomeNoisyCodewords(C,numberReceivedWords,0.5-errorRate)



    mat_rec=matrix(GF(2),sampleLinearlyIndependent(rec,C.dimension()))

    Rec=codes.LinearCode(mat_rec)



    H_c_g=Rec.systematic_generator_matrix()



    A=H_c_g[:,H_c_g.nrows():H_c_g.ncols()]

    id=H_c_g[:,:H_c_g.nrows()]

    print("generator matrix of code from received code words in systematic form?",id==Matrix.identity(GF(2),H_c_g.nrows()))

    At=A.transpose()

    H_c=block_matrix(GF(2),[[At,Matrix.identity(GF(2),At.nrows())]])



    L=SVR(Rec.parity_check_matrix(),C.length()-2,ns,w_thres,w_thres,1,GF(2))

    L_=[]

    for v in L:

        if not(v in L_):

            L_=L_+getOrbit(v,blocksize)



    return rec,L_,numErrors









        



def trySVR_fixedCode_fixedReceivedWords(C,rec,w_thres,ms,ns):

    mat_rec=matrix(GF(2),rec)

    Rec=codes.LinearCode(mat_rec)



    H_c_g=Rec.systematic_generator_matrix()

    blocksize=H_c_g.nrows()



    A=H_c_g[:,blocksize:H_c_g.ncols()]

    At=A.transpose()

    H_c=block_matrix(GF(2),[[At,Matrix.identity(GF(2),At.nrows())]])

    print("multiplication:")

    print(H_c*(H_c_g.transpose()))

    print("code length:",C.length())

    print("rows of parity check from received words:",H_c.nrows())



    L=SVR(Rec.parity_check_matrix(),C.dimension(),ns,w_thres,w_thres,100,GF(2))

    return rec,L



#C,received,guess_H,numerrors=trySVR(20,2,2,15,0,15,20)





def trySVR(codeLength,blocSize,maxWeigth,numberReceivedWords,errorRate,ms,ns):

    C=generate_QC_LDPC_code(codeLength,blocSize,maxWeigth,GF(2))

    rec,numErrors=getSomeNoisyCodewords(C,C.dimension(),0.5-errorRate)

    print("number of rec:",len(rec))

    mat_rec=matrix(GF(2),rec)

    Rec=codes.LinearCode(mat_rec)

    print("dimension of rec code",Rec.dimension())



    H_c_g=Rec.systematic_generator_matrix()

    blocksize=H_c_g.nrows() 

    print("num row in gen:",H_c_g.nrows())

    A=H_c_g[:,blocksize:H_c_g.ncols()]

    

    At=A.transpose()

    H_c=block_matrix(GF(2),[[At,Matrix.identity(GF(2),At.nrows())]])

    print("multiplication:")

    print(H_c*(H_c_g.transpose()))

    print("code dimension:",C.dimension())

    print("code length:",C.length())

    print("rows of parity check from received words:",H_c.nrows())



    L=SVR(Rec.parity_check_matrix(),C.dimension(),ns,maxWeigth,maxWeigth,10000,GF(2))



    

def reconstruction(N_iter,samples,n,k,ms,ns,circulantBlocklWidth,w_thres):

    

    C_rec=Matrix(GF(2),samples)

    mat,_,selectedColumns=sample_matrix(C_rec,ms,ns) #on prend une sous matrice des mots reçus

    perm=[i for i in range(ns)]

    print("matrice gen du code a partir de echantillon")

    print(mat)



    #il faudra trouver une idée pour cette histoire de mise sous forme systematique



    rec_dual=mat.right_kernel().basis()

    H_b=Matrix(GF(2),rec_dual) #l'orthogonal du code engendré par la sous matrice des mots reçus

    print("gen du dual:")

    print(H_b)



    H_r=SVR(H_b,ms,ns,perm,0,w_thres,N_iter,GF(2))



    

    N=len(H_r)

    for i in range(N):

        H_r[i]=extend(H_r[i],n,selectedColumns)



    L=[]

    for v in H_r:

        if not (v in L):

            L=L+getOrbit(v,circulantBlocklWidth)

    

    print("nombre de vecteurs de candidats trouvé:",len(L))



    return L

    







