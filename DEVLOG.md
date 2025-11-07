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

Beginning optimisations to how files are read.

Written a rather unfair Python version without any of the comparisons to find columns etc, just hard coded the longitude column. Both the C++ and Python finish in roughly the same time, except the C++ one also creates and deletes ~22 million stringstream objects along the way, and parses all columns looking for the correct column index...

C++:
blaze-csv ukraineflights.csv 27.1995 average
Total Count: 22517263
Valid Count: 22517263
Sum: 6.41377e+08
Average: 28.4838


--- Profiling Report ---
Program took: 29856.6ms to run.

Python: 
python py-benchmark.py
Start: 1762434265.9437895
End: 1762434296.0578191
Total: 641377135.594248
Count: 22517264

End - Start = 30.1140296 seconds

# 07.11.2025
Updated the python version to behave a lot closer to how the C++ one does to better gage performance differences, results below:

- Both got the same results
- C++ took: 18.3233 s
- Python took: 48.6279435 s

I think the largest reason between the big difference between the C++ result today (~18s) and yesterdays (~30s) is because I had more programs running etc yesterday, although on both days when I ran the Python and C++ programs they were both under the same test conditions on each day.