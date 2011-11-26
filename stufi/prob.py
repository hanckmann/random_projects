#! /usr/bin/python2

###########################################################
schuld = 100
rente1 = 0.50
betaal = 10

extra  = 10
rente2 = 0.50
###########################################################
print()

print("schuld: " + str(schuld))

###########################################################
print()

remain = schuld
monthCounter = 0

print("Month,Remain,Total")
while (remain > 0):
	monthCounter = monthCounter + 1 
	remain = remain - betaal
	remain = remain * (1 + (rente1/12))
#	print(str(monthCounter) + "," + str(remain) + "," + str(monthCounter*betaal))

print("Kosten bij minimaal terug betalen")	
print("Months: " + str(monthCounter))
print("Remain: " + str(remain))
print("Total : " + str(monthCounter*betaal+remain))

###################################################
print()

remain = schuld
monthCounter = 0

print("Month,Remain,Total")
while (remain > 0):
        monthCounter = monthCounter + 1
        remain = remain - (betaal + extra)
        remain = remain * (1 + (rente1/12))
#        print(str(monthCounter) + "," + str(remain) + "," + str(monthCounter*(betaal+extra)))

print("Kosten bij extra terug betalen (" + str(extra) + ")")
print("Months: " + str(monthCounter))
print("Remain: " + str(remain))
print("Total : " + str(monthCounter*(betaal+extra)+remain))

##################################################
print()

remain = schuld
monthCounter = 0

gespaard = 0

print("Month,Remain,Gespaard")
while (remain > gespaard):
        monthCounter = monthCounter + 1
        remain = remain * (1 + (rente1 / 12))
	remain = remain - betaal
	gespaard = gespaard * (1 + (rente2 / 12))
	gespaard = gespaard + extra
#	print(str(monthCounter) + "," + str(remain) + "," + str(gespaard))

print("Kosten bij terug betalen en extra sparen (" + str(extra) + ")")
print("Months:   " + str(monthCounter))
print("Remain:   " + str(remain))
print("Gespaard: " + str(gespaard))
print("Total : " + str((monthCounter * (betaal + extra)) + remain - gespaard))

##################################################
print()

omc = monthCounter * 2 + 1
monthCounter = 0
gespaard = 0

print("Month,Gespaard")
while (monthCounter < omc):
        monthCounter = monthCounter + 1
        gespaard = gespaard * (1 + (rente2 / 12))
        gespaard = gespaard + extra
#        print(str(monthCounter) + "," + str(gespaard))

print("Sparen " + str(extra) )
print("Months:   " + str(monthCounter))
print("Gespaard: " + str(gespaard))
print("Inleg   : " + str(monthCounter * extra))
