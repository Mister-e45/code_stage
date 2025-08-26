from sage.all_cmdline import * 

from SVR import *





def randomPermutation(n):

    L=[]

    while len(L)<n:

        x=randint(0,n-1)

        if x in L:

            continue

        else:

            L.append(x)

    return L





def partialEchelonize(M,t,perfMeasure=False): #modifie M, reduit les t premiere colonne si possible, renvoie la liste des indices des colonnes non reduites et selon perfMeasure le nombre d'additons dans GF2

    field=GF(2)

    m=M.nrows()

    nulColumns=[] #coordonées sur lesquelles le vecteur de parité devra être nul



    if perfMeasure:

        numAdditions=0

        for k in range(t):



            if M[k,k]!=field(1): #si on a pas de bon pivot

                for l in range(k+1,t):

                    if M[l,k]==field(1):

                        swapRows(M,l,k)

                        break



            if M[k,k]!=GF(2)(1): #si on a toujours pas de bon pivot

                nulColumns.append(k)

                continue #on laisse la colonne nulle

            

            for l in range(k-1,-1,-1):

                if M[l,k]==field(1):

                    numAdditions+=LinCombinationRows(M,l,k,perfMeasure)



            for l in range(k+1,m-1):

                if M[l,k]==field(1):

                    numAdditions+=LinCombinationRows(M,l,k,perfMeasure)



        return nulColumns,numAdditions



    else:

        for k in range(t):



            if M[k,k]!=field(1): #si on a pas de bon pivot

                for l in range(k+1,t):

                    if M[l,k]==field(1):

                        swapRows(M,l,k)

                        break



            if M[k,k]!=GF(2)(1): #si on a toujours pas de bon pivot

                nulColumns.append(k)

                continue #on laisse la colonne nulle

            

            for l in range(k-1,-1,-1):

                if M[l,k]==field(1):

                    LinCombinationRows(M,l,k)



            for l in range(k+1,m-1):

                if M[l,k]==field(1):

                    LinCombinationRows(M,l,k)

        return nulColumns,None





#not my code

def subsets_k(superset, k):

  if k > len(superset):

    return

  if k == 0:

    yield []

    return



  indices = list(range(k))

  while True:

    yield [superset[i] for i in indices]



    i = k - 1

    while indices[i] == len(superset) - k + i:

      i -= 1

      if i == -1:

        return

    

    indices[i] += 1

    for j in range(i + 1, k):

      indices[j] = indices[i] + j - i

      

#end of not my code





def sumRows(mat,indices,perfMeasure=False):

    if perfMeasure:

        v=0

        numAdditions=0

        for k in indices:

            v+=mat[k]

            numAdditions+=len(v) # on a dû sommer chaque coefficients du vecteur v



        return v,numAdditions

        

    else:

        v=0

        for k in indices:

            v+=mat[k]

        return v,None





def projectionMethod(n,L1,L2,l,theta,perfMeasure=False):

    I=sample(l,0,n-1)

    dictionary={}

    pairs=[]

    n1=len(L1)

    n2=len(L2)

    numAdditions=0

    for v in L1:

        k=vector(proj(v,I),immutable=True)

        if k in dictionary:

            dictionary[k].append(v)

        else:

            dictionary[k]=[]

            dictionary[k].append(v)

    

    for vecL2 in L2:

        ind=vector(proj(v,I),immutable=True)

        L=dictionary[ind]

        if L==None:

            continue

        else:

            for vecL1 in L:

                if perfMeasure:

                    numAdditions+=len(vecL1)



                if HW(vecL2-vecL1) <= theta:

                    pairs.append((vecL1,vecL2))

    if perfMeasure:

        return pairs,I,numAdditions

    else:

        return pairs,I,None















def PartialGaussianEliminationMethod(M:list,t,tau,target_w,p,ClosestNeighborAlgo,perfMeasure=False):

    field=GF(2)

    n=0

    if len(M)!=0:

        n=len(M[0])

    else:

        print("il n'y a aucun mots reçu!")

        return None,None

    numAdditions=0



    

    alpha=((1-(1-2*tau))**target_w)/2



    pi = randomPermutation(n)

    inv_pi=inversePermutation(pi)

    M_perm=[]

    for v in M:

        M_perm.append(applyPermutationOnVector(v,pi))



    M_perm=Matrix(GF(2),M_perm)



    nulCoordinates=[]





    if perfMeasure:

        nulCoordinates,number_GF2sums=partialEchelonize(M_perm,t,perfMeasure)

        numAdditions+=number_GF2sums

    else:

        nulCoordinates,_=partialEchelonize(M_perm,t,False)



    



    A=M_perm[t:,t:]

    B=M_perm[:t,t:]



    

    N=A.ncols()



    bound=p//2

    At=A.transpose()

    Bt=B.transpose()



    indices1=[k for k in range(N//2)]

    indices2=[N//2+k for k in range(N//2)]



    combinations1=list(subsets_k(indices1,bound))

    combinations2=list(subsets_k(indices2,bound))



    L1={}

    L2={}

    list1=set()

    list2=set()

    

    for comb in combinations1:

        l=[]

        if perfMeasure:

            l,number_GF2sums=sumRows(At,comb,perfMeasure)

            numAdditions+=number_GF2sums

        else:

            l,_=sumRows(At,comb)



        immutable_vector=vector(l,immutable=True)

        L1[immutable_vector]=comb

        list1.add(immutable_vector)



    for comb in combinations2:

        l=[]

        if perfMeasure:

            l,number_GF2sums=sumRows(At,comb,perfMeasure)

            numAdditions+=number_GF2sums

        else:

            l,_=sumRows(At,comb)



        immutable_vector=vector(l,immutable=True)

        L2[immutable_vector]=comb

        list2.add(immutable_vector)

    

    p1=alpha*(A.nrows())

    p2=A.nrows()/2

    theta=int((p1+p2)/2)



    L=[]

    I=[]

    if perfMeasure:

        l=int(log(len(list1))/log(2))+1

        numIter=int((1-alpha)**(-l))+1

        for k in range(numIter):

            L_temp,I,number_GF2sums=ClosestNeighborAlgo(A.ncols(),list1,list2,l,theta,perfMeasure)

            numAdditions+=number_GF2sums

            L=L+L_temp

        

    else:

        l=int(log(len(list1))/log(2))+1

        numIter=int((1-alpha)**(-l))+1

        for k in range(numIter):

            L_temp,I,_=ClosestNeighborAlgo(A.ncols(),list1,list2,l,theta,perfMeasure)

            L=L+L_temp



    H=[]

    if len(L)!=0:

        for pair in L:

            c1,c2=pair



            support=L1[c1]+L2[c2]

            v=vector(GF(2),n)



            for i in support:

                v[i+t]=1



            if perfMeasure:

                v_firstCoords,number_GF2sums=sumRows(Bt,support,perfMeasure)

                numAdditions+=number_GF2sums



                for k in range(t):

                    if v_firstCoords[k]==field(1):

                        v[k]=field(1)



            else:

                v_firstCoords,_=sumRows(Bt,support)

            

                for k in range(t):

                    if v_firstCoords[k]==field(1):

                        v[k]=field(1)

            

            for k in nulCoordinates:

                v[k]=field(0)



            



            v=applyPermutationOnVector(v,inv_pi)

            H.append(v)



    if perfMeasure:

        return H,numAdditions

    

    else:

        return H,None



def testPartialGaussianElimination(C,circulantBlocklWidth,sampleSize,errorRate,t,p,targetWeight,perfMeasure=False):



    rec,num=getSomeNoisyCodewords(C,sampleSize,errorRate)

    H=[]

    complexity=0





    if perfMeasure:

        H,number_GF2sums=PartialGaussianEliminationMethod(rec,t,errorRate,targetWeight,p,projectionMethod,perfMeasure)

        complexity+=number_GF2sums

    else:

        H,_=PartialGaussianEliminationMethod(rec,t,errorRate,targetWeight,p,projectionMethod)



    if perfMeasure and len(H)==0:

        print("aucun vecteur n'a été retrouvé")

        return None,None,None,complexity

    elif len(H)==0:

        print("aucun vecteur n'a été retrouvé")

        return None,None,None



    dualGeneratorMatrix=Matrix(H)



    print("informations sur le nombre de vecteurs de parité corrects trouvés: ")

    AllInDual(C,H)



    print("dimension de l'espace vectoriel engendré par les vecteurs de parités trouvés:",dualGeneratorMatrix.rank())





    if perfMeasure:

        return rec,num,H,complexity

    else:

        return rec,num,H











