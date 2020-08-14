# 牛顿迭代法求开方下取整
T=int(raw_input())
for i in range(0,T):
  n=int(raw_input())
  if n<2:
    print n
    continue
  m=2
  tmpn,len=n,0
  while tmpn>0:
    tmpn/=10
    len+=1
  base,digit,cur=300,len/m,len%m
  while(cur+m<= base)and(digit > 0):
    cur+=m
    digit-=1
  div=10**(digit*m)
  tmpn=n/div
  x =int(float(tmpn)**(1.0/m))
  x*=(10**digit)
  while True:
    x0=x
    x=x+x*(n-x**m)/(n*m)
    if x==x0:break
  while (x+1)**m<=n:
    x=x+1
  print x