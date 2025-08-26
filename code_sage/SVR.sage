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


def generate_QC_LDPC_code(n,circulantMatricesShape,maxWeight,field=GF(2)):
    rows,cols=circulantMatricesShape
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
    
    
    return C,mat

def generate_QC_LDPC_code_constWeight(n,circulantMatricesShape,Weight,field=GF(2)):
    rows,cols=circulantMatricesShape
    if rows>cols:
        print("wrong circulant block size rows must be at most equal to the numer of columns")
    
    V=[vector(GF(2),n) for k in range(rows)]
    supports=[pickDistinctInRange(0,n-1,Weight) for k in range(rows)]
    for k in range(rows):
        for ind in supports[k]:
            V[k][ind]=GF(2)(1)
 
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
    
    
    return C,mat


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
    return lines,colums

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
        L.append(p2[p1[i]])
    return L


def transposition(i,j,n):
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

def swapColums(M,i,j):
    M_l=[]
    for k in range(M.nrows()):
        M_l.append(M[k])
    
    swapM_l=[]
    for v in M_l:
        swapM_l.append(VectorSwapCoordinates(v,i,j))
    
    return matrix(swapM_l)



def swapCols(M,i,j): #modifie l'entrée
    n=M.nrows()
    v=[M[k,i] for k in range(n)]
    for l in range(n):
        M[l,i]=M[l,j]
        M[l,j]=v[l]

def LinCombinationRows(M,i,j,perfMeasure=False): #modifie l'entrée et la ligne i vaut elle même plus la ligne j

    if perfMeasure:
        numberSums=0
        n = M.ncols()
        for k in range(n):
            M[i,k]+=M[j,k]
            numberSums+=1

        return numberSums
    else:
        n = M.ncols()
        for k in range(n):
            M[i,k]+=M[j,k]

        return None
        


def swapRows(M,i,j):
    v=list(M[i])
    n=len(v)
    for k in range(n):
        M[i,k]=M[j,k]
        M[j,k]=v[k]

def findLinePivot(M,col): #retourne -1 si aucun pivot n'est trouvé
    n=M.nrows()
    for i in range(col,n):
        if M[i,col]!=0:
            return i

    return -1

def findNonZeroUnder(M,col,index): #trouve un coefficient non nul en dessous d'une certaine ligne sur une colonne fixée, retourne -1 si aucun élément n'est trouvé
    n=M.nrows()
    for i in range(index,n):
        if M[i,col]!=0:
            return i
    return -1



def applyPermutationOnVector(v,sigma): #permute les coordonées de v selon sigma
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
    


def reduce(M,perfMeasure=False): #reduit et retourne les permutations effectuées sur les colonnes 
    
    numberOperations=0
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
                    t=transposition(i,k,n)
                    colsPermutation=composePermutation(t,colsPermutation)
                    found=True
                    break
            if not found:
                print("impossible de continuer la reduction car plus aucune colonne non nulle n'est disponible")
                return E,colsPermutation
        
        pivotRow=findLinePivot(E,i)
        if pivotRow==-1:
            found=False
            for k in range(i+1,n):
                pivotRow=findNonZeroUnder(E,k,i)
                if pivotRow!=-1:
                    swapCols(E,k,i)
                    t=transposition(i,k,n)
                    colsPermutation=composePermutation(t,colsPermutation)
                    found=True
                    break
            if not found:
                print("colonne avec un bon pivot introuvable")

        if pivotRow!=i:
            swapRows(E,i,pivotRow) 
            

        for k in range(i+1,m): #on annule tout les coeff sous le pivot
            if E[k,i]!=0:
                if perfMeasure:
                    numberOperations+=LinCombinationRows(E,k,i,perfMeasure)
                else:
                    LinCombinationRows(E,k,i)
                
    

    for i in range(m-1,-1,-1):
        for k in range(i):
            if E[k,i]!=0:
                if perfMeasure:
                    numberOperations+=LinCombinationRows(E,k,i,perfMeasure)
                else:
                    LinCombinationRows(E,k,i)
                
        
        
                
    if perfMeasure:
        return E,colsPermutation,numberOperations
    else:
        return E,colsPermutation,None

def reduce_perfMeasure(M): #reduit et retourne les permutations effectuées sur les colonnes 
    rowOperations=0
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
                    t=transposition(i,k,n)
                    colsPermutation=composePermutation(t,colsPermutation)
                    found=True
                    break
            if not found:
                print("impossible de continuer la reduction car plus aucune colonne non nulle n'est disponible")
                return E,colsPermutation
        
        pivotRow=findLinePivot(E,i)
        if pivotRow==-1:
            found=False
            for k in range(i+1,n):
                pivotRow=findNonZeroUnder(E,k,i)
                if pivotRow!=-1:
                    swapCols(E,k,i)
                    t=transposition(i,k,n)
                    colsPermutation=composePermutation(t,colsPermutation)
                    found=True
                    break
            if not found:
                print("colonne avec un bon pivot introuvable")

        if pivotRow!=i:
            swapRows(E,i,pivotRow) 
            rowOperations+=1

        for k in range(i+1,m): #on annule tout les coeff sous le pivot
            if E[k,i]!=0:
                LinCombinationRows(E,k,i)
                rowOperations+=1
    

    for i in range(m-1,-1,-1):
        for k in range(i):
            if E[k,i]!=0:
                LinCombinationRows(E,k,i)
                rowOperations+=1
        
        
                
    
    return E,colsPermutation,rowOperations

def left_reduce(M,perfMeasure=False): #pareil que la reduction mais le bloc identité sera à droite, retourne aussi les eventuelles permutations effectuées sur les colonnnes
    n=M.ncols()
    m=M.nrows()
    colsPermutation=[i for i in range(n)] 
    E=copy(M)
    numberAdditions=0
    for i in range(m):
        if columnIsZero(E,n-m+i):
            found=False
            for k in range(0,n-m+i):
                if not columnIsZero(E,n-m+k):
                    swapCols(E,k,n-m+i)
                    t=transposition(n-m+i,k,n)
                    colsPermutation=composePermutation(t,colsPermutation)
                    found=True
                    break
            if not found:
                print("impossible de continuer la reduction car plus aucune colonne non nulle n'est disponible(left reduce)")
                return E,colsPermutation
        
        pivotRow=findNonZeroUnder(E,n-m+i,i)
        if pivotRow==-1:
            found=False
            for k in range(n-m+i+1,n-m+i+n):
                pivotRow=findNonZeroUnder(E,k%n,i)
                if pivotRow!=-1:
                    found=True
                    swapCols(E,k%n,n-m+i)
                    t=transposition(n-m+i,k%n,n)
                    colsPermutation=composePermutation(t,colsPermutation)
                    break
            if not found:
                print("colonne avec un bon pivot introuvable (left reduce)")
            

        if pivotRow!=i:
            swapRows(E,i,pivotRow)
            

        for k in range(i+1,m):
            if E[k,n-m+i]!=0:
                if perfMeasure:
                    numberAdditions+=LinCombinationRows(E,k,i,perfMeasure)
                else:
                    LinCombinationRows(E,k,i,perfMeasure)

    for i in range(0,m): 
        for k in range(i-1,-1,-1):
            if E[k,n-m+i]!=0:
                if perfMeasure:
                    numberAdditions+=LinCombinationRows(E,k,i,perfMeasure)
                else:
                    LinCombinationRows(E,k,i,)
    
    if perfMeasure:
        return E,colsPermutation,numberAdditions
    else:
        return E,colsPermutation,None


def left_reduce_perfMeasure(M): #pareil que la reduction mais le bloc identité sera à droite, retourne aussi les eventuelles permutations effectuées sur les colonnnes
    rowOperations=0
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
                    t=transposition(n-m+i,k,n)
                    colsPermutation=composePermutation(t,colsPermutation)
                    found=True
                    break
            if not found:
                print("impossible de continuer la reduction car plus aucune colonne non nulle n'est disponible(left reduce)")
                return E,colsPermutation
        
        pivotRow=findNonZeroUnder(E,n-m+i,i)
        if pivotRow==-1:
            found=False
            for k in range(n-m+i+1,n-m+i+n):
                pivotRow=findNonZeroUnder(E,k%n,i)
                if pivotRow!=-1:
                    found=True
                    swapCols(E,k%n,n-m+i)
                    t=transposition(n-m+i,k%n,n)
                    colsPermutation=composePermutation(t,colsPermutation)
                    break
            if not found:
                print("colonne avec un bon pivot introuvable (left reduce)")
            

        if pivotRow!=i:
            swapRows(E,i,pivotRow)

        for k in range(i+1,m):
            if E[k,n-m+i]!=0:
                LinCombinationRows(E,k,i)
                rowOperations+=1

    for i in range(0,m): 
        for k in range(i-1,-1,-1):
            if E[k,n-m+i]!=0:
                LinCombinationRows(E,k,i)
                rowOperations+=1
        
    return E,colsPermutation,rowOperations
        
def proj(v,indices): #on ne garde que les coordonées dans "indices" 
    indices.sort()
    V=[]
    for i in indices:
        V.append(v[i])
    return vector(V)

def extend(v,n,indices): #on complete avec des 0 en conservant les coordonee dans "indice" à la bonne place
    V=[0 for i in range(n)]
    N=len(indices)
    for i in range(N):
        V[indices[i]]=v[i]
    return vector(V)

    
def amputatedSVR(H_b,ms,ns,permutation,d_s,w_thres,N_iter,field): #SVR sans la partie "filtering"
    H_r=[]
    perm=copy(permutation)
    H_c=copy(H_b)
    for i in range(N_iter): 
        inv_perm=inversePermutation(perm) 

        for u in range(len(list(H_c))):
            for v in range(u,len(list(H_c))):
                s=H_c[u]+H_c[v] 
                if HW(s)+2<=w_thres and H_c[u]!=H_c[v]: 
                    H_r.append(applyPermutationOnVector(s,inv_perm))

        j=randint(0,ms-1)
        k=randint(ms,ns-1)
        
        
        H_c=swapColums(H_c,j,k)
        t=transposition(j,k,ns)
        perm = composePermutation(t,perm)
        
        H_c,p=left_reduce(H_c)
        
    
        perm=composePermutation(p,perm)

    return H_r

def SVR(H_b,ms,ns,permutation,d_s,w_thres,N_iter,perfMeasure=False,field=GF(2)):
    H_r=[]
    perm=copy(permutation)

    H_c=copy(H_b)
    fracProgress=N_iter//10
    progress=0
    complexity=0
    for i in range(N_iter): 
        progress+=1
        if progress%10 == 0: #un petit compteur pour dire à quelle itération l'algorithme est
            print(progress,"/",N_iter)

        inv_perm=inversePermutation(perm) 

        X=[]
        d=genRandomBinaryVector(ns)
        L=pickDistinctInRange(0,ms-1,d_s)
        for k in range(ns-ms):
            eq=True
            for j in L:
                if H_c[k,j]==d[j]:
                    continue
                else:
                    eq=False
                    break
            if eq:
                X.append(H_c[k])
        lenX=len(X) 

        for u in range(lenX):
            for v in range(u,lenX):
                s=X[u]+X[v]
                complexity+=1
                if HW(s)+2<=w_thres and X[u]!=X[v]:
                    H_r.append(applyPermutationOnVector(s,inv_perm))

        j=randint(0,ms-1)
        k=randint(ms,ns-1)
        H_c=swapColums(H_c,j,k)
        t=transposition(j,k,ns)

        perm = composePermutation(t,perm)
        if perfMeasure:
            H_c,p,compl=left_reduce(H_c,perfMeasure)
            complexity+=compl
            perm=composePermutation(p,perm)
        else:
            H_c,p,_=left_reduce(H_c,perfMeasure)
            perm=composePermutation(p,perm)

    if perfMeasure:
        return H_r,complexity
    else:
        return H_r,None


def SVR_perfMeasure(H_b,ms,ns,permutation,d_s,w_thres,N_iter,field):
    rowOperations=0
    H_r=[]
    perm=copy(permutation)

    H_c=copy(H_b)
    fracProgress=N_iter//10
    progress=0
    for i in range(N_iter): 
        progress+=1
        if progress%10 == 0: #un petit compteur pour dire à quelle itération l'algorithme est
            print(progress,"/",N_iter)

        inv_perm=inversePermutation(perm) 

        X=[]
        d=genRandomBinaryVector(ns)
        L=pickDistinctInRange(0,ms-1,d_s)
        for k in range(ns-ms):
            eq=True
            for j in L:
                if H_c[k,j]==d[j]:
                    continue
                else:
                    eq=False
                    break
            if eq:
                X.append(H_c[k])
        lenX=len(X) 

        for u in range(lenX):
            for v in range(u,lenX):
                s=X[u]+X[v]
                rowOperations+=1
                if HW(s)+2<=w_thres and X[u]!=X[v]:
                    H_r.append(applyPermutationOnVector(s,inv_perm))

        j=randint(0,ms-1)
        k=randint(ms,ns-1)
        H_c=swapColums(H_c,j,k)
        t=transposition(j,k,ns)

        perm = composePermutation(t,perm)

        H_c,p,row_op=left_reduce_perfMeasure(H_c)
        rowOperations+=row_op
        perm=composePermutation(p,perm)

    return H_r,rowOperations

def AllInDual(C,L): #une fonction pour tester si une liste de vecteur "L" est dans le dual d'un code "C"
    w=0
    g=True
    for v in L:
        if not(v in C.dual_code()):
            w+=1
            g=False

    print("nombre de mots pas dans le dual:",w)
    print("nombre de mots total:",len(L))
    print("proportion des mots à l'exterieur du dual:",float(w/len(L)))
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


def is_systematicForm(M):
    m=M.nrows()
    Id=M[:,[k for k in range(m)]]
    return Id==Matrix.identity(GF(2),m) 


def argmax(L): #donne les indices des maximum de L
    arg=[0]
    m=L[0]
    N=len(L)
    for k in range(1,N):
        c=L[k]
        mtemp=m
        if m==c:
            arg.append(k)
            mtemp=c

        if m<c: 
            arg=[k]
            mtemp=c 

        m=mtemp

    return arg




def bit_flip_partialCorrection(v,H,perfMeasure=False,sparseMat=True):
    F=GF(2)
    V=copy(v)
    N=len(V)
    numberOperations=0

    mat_H=Matrix(F,H,sparse=sparseMat)
    syndrome=mat_H*V
    if perfMeasure:
        numberOperations+=2*N-1
    count=0
    while syndrome!=0:
        count+=1
        if count>10:
            break

        num_failed_parityCheck_for_bit_i=[0 for k in range(N)]
        
        n=len(syndrome)
        for k in range(n):
            if syndrome[k] == 1:
                for l in range(N):
                    if mat_H[k,l]==1:
                        num_failed_parityCheck_for_bit_i[l]+=1
                        if perfMeasure:
                            numberOperations+=1

        index_of_max=argmax(num_failed_parityCheck_for_bit_i)
        
        if len(index_of_max)==0:
            print("quelque chose cloche!")
        else:
            V[index_of_max[0]]+=F(1)
            if perfMeasure:
                numberOperations+=1
            

        syndrome=mat_H*V
        if perfMeasure:
            numberOperations+=2*N-1
    if perfMeasure:
        return V,numberOperations
    else:
        return V,None


def codeFromMat(M):
    H=Matrix(M)
    return codes.LinearCode(H)
    

def reconstruction(N_iter,samples,n,k,ms,ns,circulantBlocklWidth,w_thres,d_s,LIM,perfMeasure=False):
    numberOperations=0
    C_rec=Matrix(GF(2),samples)
    partial_decoded_C_rec=copy(C_rec)
    sampleSize=len(samples)
    it=0
    L=[]
    numrank=0
    foundNew=False
    rank=0
    while numrank<n-k:
        
        H_r=[]
        it+=1
        if it>LIM: #LIM est le nombre maximal d'itération 
            break

        selectedLines,selectedColumns=sample_matrix(partial_decoded_C_rec,ms,ns) #on prend une sous matrice des mots reçus

        if foundNew: #si de nouveau vecteurs creux sont trouvés
            foundNew=False
            print("correction partielle de l'ensemble des mots reçus")
            compte=0
            for l in range(C_rec.nrows()): #on effectue un decodage partiel sur les mots reçus
                partial_decoded_C_rec[l],compl=bit_flip_partialCorrection(C_rec[l],L,perfMeasure)
                if perfMeasure:
                    numberOperations+=compl


            print("correction partielle terminée")    

        print("echantillonage parmi les mots") 
        mat=partial_decoded_C_rec[selectedLines,selectedColumns] 
        mat=Matrix(GF(2),mat)

        E,perm,compl=reduce(mat,perfMeasure)
        if perfMeasure:
            numberOperations+=compl

        print("reduction de la matrice echantillonée effectuée") 
        A=E[:,ms:]
        H_b=block_matrix(GF(2),1,2,[[A.transpose(),Matrix.identity(GF(2),ns-ms)]]) #une matrice de parité obtenue depuis la forme systématique : [At | Id]
        print("echantillonage et calcul d'une matrice de parité effectués")

        print("recherche de vecteurs creux dans le dual de l'échantillon")
        H_r,compl=SVR(H_b,ms,ns,perm,d_s,w_thres,N_iter,perfMeasure,GF(2)) #recherche de vecteur de parité creux à partir de l'échantillon
        if perfMeasure:
            numberOperations+=compl
        
        
        N=len(H_r)
        lenL=len(L)
        rank=matrix(GF(2),L,sparse=True).rank() #le rang de l'espace dual couramment détenu
        if perfMeasure:
            if len(L)!=0:
                numberOperations+=(len(L[0])*len(L))**3

        for i in range(N):
            H_r[i]=extend(H_r[i],n,selectedColumns) #on reconstitue des vecteurs de parité du code en les plongeant dans l'espace de bonne dimension

        print("nombre de vecteurs candidats trouvés à l'iteration:",it,"est",len(H_r)*n//circulantBlocklWidth)
        M=matrix(GF(2),L+H_r,sparse=True) #rang de l'espace dual auquel on a ajouté les vecteurs creux découverts
        new_r=M.rank()
        if perfMeasure:
            numberOperations+=(M.ncols()*M.nrows())**3
        if new_r>rank: # si on a gagné en dimension
            for v in H_r: #on va determiner lesquels apportent cet ajout de dimension pour ne garder qu'une famille de taille minimale
                if not (v in L):
                    O=getOrbit(v,circulantBlocklWidth) #on tire profit de la structure du code pour trouver d'autres vecteurs de parité
                    M=matrix(GF(2),L+O)
                    new_r=M.rank()
                    if perfMeasure:
                        numberOperations+=(M.ncols()*M.nrows())**3
                    if new_r>rank:
                        L=L+O
                        rank=new_r
            
            
        numrank=rank
        
        if len(L)>lenL: 
            foundNew=True # a la prochaine itération il faudra faire un débruitage partiel des mots

        print("nombre de vecteurs candidats retenus à l'iteration:",it,"est",len(L)-lenL)
        print("nombre de vecteur de parité trouvés jusqu'à présent:",len(L))
        print("...qui forment un espace de dimension:",numrank)

        
        
    if perfMeasure:
        return L,numberOperations
    else:
        return L,None



    

def testRecoveryAlgorithm(n,blockSize,maxWeigth,errorRate,N_iter,w_thres,LIM,perfMeasure=False):#retourne un quadruplet: Le code QC-LDPC généré aléatoirement, les mots reçus générés, le nombre de bits erronés, et les vecteurs de parité trouvé

    rowOperations=0
    l=randint(1,blockSize-1)
    C,_=generate_QC_LDPC_code(n,(l,blockSize),maxWeigth)
    print("longueur du code généré:",C.length())
    print("dimension du code généré:",C.dimension())

    print("entrez le nombre de mots reçus")
    sampleSize=int(input())

    rec,num=getSomeNoisyCodewords(C,sampleSize,errorRate)
    print("mots reçus générés, il y a",num, "bits erronés")
    k=C.dimension()
    print("la dimension du code est",k, "entrez ms (nombre de mots echantillonnés dans parmi les mots reçus)")
    ms=input()
    print("entrez ns (nombre de colonnes echantillonnées)")
    ns=input()
    ms=int(ms)
    ns=int(ns)
    print("entrez la valeur de d_s (on cherchera les vecteurs creux comme somme de deux vecteurs de parité egaux sur d_s coordonées)")
    d_s=int(input())
    if perfMeasure:
        H,row_op=reconstruction(N_iter,rec,n,k,ms,ns,blockSize,w_thres,d_s,LIM)
        rowOperations+=row_op
    else:
        H=reconstruction(N_iter,rec,n,k,ms,ns,blockSize,w_thres,d_s,LIM)

    if len(H)==0:
        print("aucun vecteur de parité n'a été trouvé")
        return None,None,None,None

    C_=codeFromMat(H)

    print("informations sur le nombre de vecteurs de parité corrects trouvés:")
    AllInDual(C,H)

    C_=codeFromMat(H)
    print("dimension de l'espace vectoriel engendré par les mots trouvés:",C_.dimension())

    if not C==C_.dual_code():
        print("le code généré aléatoirement n'a pas été reconstruit")
    else:
        print("Le code genéré aléatoirement a pu être reconstruit!")
        
    if perfMeasure:
        return C,rec,num,H,rowOperations
    else:
        return C,rec,num,H



def testRecoveryAlgorithmFixedCode(C,blockSize,errorRate,N_iter,w_thres,LIM,perfMeasure=False): #retourne un triplet: les mots reçus générés, le nombre de bits erronés, et les vecteurs de parité trouvé
    print("longueur du code:",C.length())
    print("dimension du code:",C.dimension())
    n=C.length()

    numberOperations=0

    print("entrez le nombre de mots reçus")
    sampleSize=int(input())

    rec,num=getSomeNoisyCodewords(C,sampleSize,errorRate)
    print("mots reçus générés, il y a",num, "bits erronés")
    k=C.dimension()
    print("la dimension du code est",k, "entrez ms (nombre de mots echantillonnés dans parmi les mots reçus)")
    ms=input()
    print("entrez ns (nombre de colonnes echantillonnées)")
    ns=input()
    ms=int(ms)
    ns=int(ns)
    print("entrez la valeur de d_s (on cherchera les vecteurs creux comme somme de deux vecteurs de parité egaux sur d_s coordonées)")
    d_s=int(input())

    d_s=10
    H=[]

    H,compl=reconstruction(N_iter,rec,n,C.dimension(),ms,ns,blockSize,w_thres,d_s,LIM,perfMeasure)
    if perfMeasure:
        numberOperations+=compl

    if perfMeasure and len(H)==0:
        return None,None,None,numberOperations

    if len(H)==0:
        print("aucun vecteur de parité n'a été trouvé")
        return None,None,None

    C_=codeFromMat(H)

    print("informations sur le nombre de vecteurs de parité corrects trouvés ")
    AllInDual(C,H)

    C_=codeFromMat(H)
    print("dimension de l'espace vectoriel engendré par les mots trouvés:",C_.dimension())

    if not C==C_.dual_code():
        print("le code n'a pas été reconstruit")
    else:
        print("Le code a pu être reconstruit!")

    if perfMeasure:
        return rec,num,H,rowOperations
    else:
        return rec,num,H,None





            

def test_partialCorrection(C,H:list,numberOfTest,errorRate):
    codeWords=[]
    noisyCodeWords=[]
    dim=C.dimension()
    N=C.length()
    numberFlawedWords=0
    for k in range(numberOfTest):
        w=C.encode(genRandomBinaryVector(dim))
        e=genErrorVector(N,errorRate)
        if e!=0:
            numberFlawedWords+=1
        codeWords.append(w)
        noisyCodeWords.append(w+e)
    
    print("génération des mots terminée")

    L=[]
    for v in noisyCodeWords:
        L.append(bit_flip_partialCorrection(v,H))
    
    N=len(L)
    success=0
    fail=0
    for k in range(N):
        if L[k]==codeWords[k]:
            success+=1
        else:
            fail+=1
    
    return success,fail,numberFlawedWords,numberOfTest

