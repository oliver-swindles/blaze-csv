# 05.11.2025
Ran a basic profiler to time how long it would take to read and print every line of the ukraineflights.csv file. 393144ms.

Not printing any of the data takes 5546.65ms to parse the file.
Tracking how many lines (22517263) takes 5047.22ms (less than before??).
This difference is negligle, likely depending heavily on whatever else my system is doing tbh.

Means that for calculating the count (+ maybe avg) later on should be fairly quick already.

# 06.11.2025
Added a simple count and sum for the column specified by the user.
When used on the ukraineflights data, longitude avg took 27074.1ms to run, and calculate the average to be 28.4838; Latitude took 39634.8ms to run, calculated average of 47.9795. The total amount of data for each was relatively similar, there were 22517263 rows of long, and 22517263 of lat.

Removed continue statements, lat now takes 30165.3ms, long now takes 27265.2ms.