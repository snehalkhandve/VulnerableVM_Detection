import os
path_to_rules = "/root/Hrishikesh/Breach_impact/rules/"
files = []
for r, d, f in os.walk(path_to_rules):
	for file in f:
		files.append(os.path.join(r, file))
l = len(files)
for i in range(0,l):
	f=files[i]
	print(f[-1])
