numset = { int(line) for line in open("sum.txt") }

target_range = range(-10000, 10000 + 1) # Inclusive
targetset = set()

print("Initialized!")

divisor_dict = {}
divisor = max(target_range) - min(target_range)
for a in numset:
    divisor_index = int( a / divisor )
    if divisor_index in divisor_dict:
        divisor_dict[divisor_index].append(a)
    else:
        divisor_dict[divisor_index] = [a]

print("Formed dict!")

member_set = set()
for a in numset:
    if a in member_set: pass
    targetlist = []
    divisor_index = int( ( min(target_range) - a ) / divisor )
    for index in range(divisor_index - 1, divisor_index + 2): # Check [divisor_index - 1, divisor_index +1]
        if index in divisor_dict:
            targetlist.extend(divisor_dict[index])
    # print("targetlist: {}".format(targetlist))
    for b in targetlist:
        if a + b in target_range:
            # print("FOUND!!! {} + {} = {}".format(a,b,a+b))
            targetset.add( a + b )
            member_set.add(a) ## Already sent a, which should only be found once
            member_set.add(b)

print(len(targetset))
print('\a')
