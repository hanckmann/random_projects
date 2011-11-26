#! /usr/bin/python2

schuld = 42000
rente1 = 0.0225
betaal = 300

spaar  = 100

remain = schuld
monthCounter = 0

while (remain > 0):
	monthCounter = monthCounter + 1 
	remain = remain - betaal
	remain = remain * (1 + (rente1/12))
	print("M:",monthCounter," R:",remain)
	
print("Months: " , monthCounter)
print("Remain: " , remain)
print("Total : " , monthCounter*betaal-remain)
