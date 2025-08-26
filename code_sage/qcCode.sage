import sage







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

    shifted=v.copy()

    for i in range(n):

        shifted[(i+shift)%n]=v[i]



    return shifted





def generate_QC_LDPC_code(n,blocSize,maxWeight,field):

    V=randomSparseVector(n,maxWeight)

    print("sparse vector :",V)

    M=[V]

    print("generated random sparse vector")

    for i in range(n//blocSize):

        M.append(cyclicRightShift(M[-1],blocSize))

    print("performed all cyclic shifts on it")

    mat=matrix(field,M)

    dualC=codes.LinearCode(mat)

    print("created the dual code")

    genC=dualC.generator_matrix().right_kernel().basis()

    print("computed a generator family of C",len(genC))

    matC=matrix(field,genC)

    C=codes.LinearCode(matC)   

    print("C is created")

    

    return C