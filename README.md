# RMemTest

export CPLUS_INCLUDE_PATH=/usr/lib64/R/include

export LD_LIBRARY_PATH=/usr/lib64/R

export R_HOME=/usr/lib64/R

make foo

./foo

[check the memory usage by pmap pid, 15M reported]

[type ENTER]

[check the memory usage by pmap pid, 234M reported]

[type ENTER]

[check the memory usage by pmap pid, 242M reported]

It seems the first 200M+ memory increase betwen the first two memory check is due to R initialization.

8M allocation between the last two memory check is due to the vector allocation.

However, no decrease of memory is observed though the R related method calling (i.e. testRMem())is done.

The test is conducted on Redhat Enterprise Linux server 7.9 with R 4.0.4
