import os
import subprocess
import pandas as pd

if os.geteuid() != 0:
	exit("You need to have root privileges to run this script.\nPlease try again using 'sudo'. Exiting.")

path_to_rules = "/root/Hrishikesh/Breach_impact/rules_csv/"
df1=pd.read_csv(os.path.join(path_to_rules, "1.csv"))
l=len(df1)
for i in range(0,l):
	address=df1['destination ip'].iloc[i]
	if address == "-":
		address = "192.168.10.0/24"
	print(address)
	protocol=df1['protocol'].iloc[i]
	print(protocol)
	decision=df1['ALLOW/DENY'].iloc[i]
	if decision == "ALLOW":
		decision = "ACCEPT"
	else:
		decision = "DROP"
	print(decision)
	p = subprocess.Popen(["iptables", "-A", "INPUT", "-p", protocol, "-s", address, "-j", decision], stdout=subprocess.PIPE)
	print('\n')
