import sys
import time


start = time.time()

# Read args
if (len(sys.argv) != 4):
    print("Incorrect number of arguments passed. Please try again.")
    sys.exit()

# Set arguments
filename = sys.argv[1]
columnName = sys.argv[2]
columnIndex = -1
operation = sys.argv[3]

# Open file
file = open(filename, "rt")   

columnHeaders = file.readline()
columnHeaders = columnHeaders.split(",")

i = 0
for header in columnHeaders:
    if (header == columnName):
        columnIndex = i
        break
    i += 1

if (columnIndex == -1):
    print("Column '" + str(columnName) + "' not found.")
    sys.exit()

fileDataLine = ""
totalSum = 0
validDataCount = 0
totalCount = 0

for line in file:
    totalCount += 1
    currentColumn = 0
    line = line.split(",")
    for column in line:
        if currentColumn == columnIndex:
            try:
                totalSum += float(column)
                validDataCount += 1
            except:
                  print("Error")
        currentColumn += 1


# for line in file:
#     line = line.split(",")
#     count += 1
#     # print(line[5] + "\n")
#     total += float(line[5])

file.close()
end = time.time()
print("Start: " + str(start))
print("End: " + str(end))
print("Total: " + str(totalCount))
print("Valid Count: " + str(validDataCount))
print("Sum: " + str(totalSum))
print("Average: " + str(totalSum / validDataCount))
        