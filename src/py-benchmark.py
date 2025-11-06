import time

start = time.time()
print("Start: " + str(start))

count = 0
total = 0

with open("../build/ukraineflights.csv") as file:
    for line in file:
        line = line.split(",")
        count += 1
        # print(line[5] + "\n")
        total += float(line[5])

end = time.time()
print("End: " + str(end))
print("Total: " + str(total))
print("Count: " + str(count))
        