import re

file = open('testout.txt')
data = file.read()

filter1 = re.findall('#compares = \d+', data)

comp_before = []
comp_after = []
choose = 0
for s in filter1:
    comparisons = re.search('\d+', s).group()
    if choose == 0:
        comp_before.append(int(comparisons))
        choose = 1
    else:
        comp_after.append(int(comparisons))
        choose = 0

filter2 = re.findall('depth = \d+', data)
depth = []
for s in filter2[0::2]:
    depth.append(re.search('\d+', s).group())

'''filter2 = re.findall('Search for \d+ values known to be in tree', data)
values_in = []
for s in filter2:
    values_in.append(re.search('\d+', s).group())

filter3 = re.findall('Search for \d+ values known to \*not\* be in tree', data)
values_not_in = []
for s in filter3:
    values_not_in.append(re.search('\d+', s).group())
'''
values_in = 5000
values_not_in = 1666

result = []
for a, b in zip(comp_before, comp_after):
    result.append(round((b - a) / (values_in + values_not_in), 2))

print(result)
print(depth)
