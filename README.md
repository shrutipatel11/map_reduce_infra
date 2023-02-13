# MapReduce Infrastructure
Name : Shruti Patel
GTID : 903710704

## Implementation

### GRPC protocol between the master and worker
* Each file shard has a unique identifier and the shard details. Shard details constitutes of the filename and the offset (start byte and end byte).
* Each map request is given a file shard as input.
* Each reduce request is given a output directory, a unique identifier for the output file (that it has to write to) and the filenames of the intermediate files (that it has to process)
* Each master request is either a map request or reduce request. 
* Each worker response is a list of filenames that the worker has written to. It can be an intermediate file or the final output file.
* A map-reduce request takes a master request as inout and returns worker response as output.

### Sharding
* The shard size and the input files are taken as an input from configuration file. This information is used to calculate the number of shards 'M'.
* Each input file is read line by line. When the number of bytes read increases the shard size, the filename and the offsets are appended to the file shards.
* The input shards have complete record entries.The shard offsets are selected in a way that no word is split in the middle. It is aligned to the next '\n' character.


### Master Worker
The master worker is a special worker which assigns map and reduce tasks to the available workers. It keeps tracks of the number of workers occupied, number of file shards processed, number of output directories created, failed workers and slow workers.

#### Map task
* The master creates a intermediate directory before the start of the map reduce task to store the intermediate files created by the map tasks.
* The mapper assigns a map request to each worker. The map request contains the user id, and the phase ("map") along with the shard details. A new GRPC call is made for each of the shard.
* If the number of map tasks 'M' is less than the number of workers, then a few workers will remain free. 
* If the number of map tasks (i.e. the number of shards) 'M' is greater than the number of workers, then it waits for the workers to repond. Once a worker responds, then it is marked as free and a new map task is assigned to it.
* When the worker response is received, the filenames received as response are stored in a hashmap. Hashmap sorts the values itself and hence, nothing extra needs to be done for sorting the intermediate files.
* The master terminates the map phase when it has processed all the shards and all the workers are free.

#### Reduce task
* The master creates a output directory to store the output files produced by the reduce tasks.
* A reducer reads each intermediate file produced by the map tasks in alphabetical order of their names. Note that a intermediate file is created for each unique word present in the input files.
* Since each reduce tasks should have equal amount of load, each reduce tasks gets approximately (number of intermediate files / number of output files) files to process. The GRPC call request contains the output directory, the identifier of the output file to write to and the filenames to read from.
* If the number of reduce tasks tasks 'R' is less than the number of workers, then a few workers will remain free. 
* If the number of reduce tasks (i.e. the number of shards) 'R' is greater than the number of workers, then it waits for the workers to repond. Once a worker responds, then it is marked as free and a new reduce task is assigned to it.
* The master terminates the reduce task once it has processed all the intermediate files and written to all output files.

#### Handling worker failure
* Whenever the status of the GRPC call is not 'OK', it meas that the worker has failed due to some reasons. In such a case, the master removes the failed worker from the list of workers.
* It then assigns the failed worker task (map/reduce) to the next available worker.
* I have assumed that a failed worker ever returns again. Hence, a task is never scheduled on the worker again.

#### Handling slow worker
* For each GRPC call, a deadline of 10s is kept. I have chosen a deadline of 10s to account for various shard sizes and the time taken to switch from worker to worker. A deadline of 5s would suffice for all cases except when the number of workers and reducers are 16.
* On detection of a slow worker, the slow worker is removed from the lost of workers available. The output produced by the slow worker is ignored and its task is assigned to a new worker.
* I have assumed that the slow worker will always remain slow and hence not task is scheduled on that worker after detection.


### Worker
A worker can perform a map or a reduce job based on the request received from the master worker.

#### Map Worker
* If a map request is received from the master, it reads the request parameters which contains the unique shard identifier and the shard details.
* It reads the input files to create a string containing all the workds without the '\n' character.
* The string is passed as an input to the user map function.

#### Reduce Worker
* If a reduce request is received from the master, it reads the request parameters which contains the output directory, the unique identifier for output file to be written to and the intermediate files to read from.
* It reads the intermediate files and gets the keys and the list of values of that key. The user reduce function is called for each intermediate file with the key and list of values for that key.


## Emit function
The emit function's input is a key and a value.

### Map emit function
* The map emit function creates an intermediate file as "key".txt. if it does not exist. It then adds the value to the file.
* If the file already exists, then it appends the value to a new line.

### Reduce emit function
* The reduce emit function write the key and value pair to the output file specified by the worker.


## Example
Shard size = 5000 bytes
Number of workers = 2
Number of output files = 2

### Input files
file1.txt (2500 bytes)
    my name is shruti

file2.txt (2500 bytes)
    my name is not shruti

file3.txt (2500 bytes)
    i study at georgia tech.

### Shards
Shard1 : (1, (("file1.txt",0 2500), ("file2.txt", 0, 2500)))
Shard2 : (2, (("file3.txt",0 2500)))

### Map tasks
Worker1: 
    Assigned shard : Shard1
    Input for user map function : "my name is shruti my name is not shruti"
    Intermediate files created : my.txt, name.txt, is.txt, not.txt, shruti.txt
    Example of my.txt contents:
    1
    1

Worker2:
    Assigned shard : Shard2
    Input for user map task : "i study at georgia tech."
    Intermediate files created : i.txt, study.txt, at.txt, georgia.txt, tech.txt

Hashmap contents after the execution of map tasks:
The key is the name of the intermediate file and the value is set to 1 by default.
[{at.txt,1}, {georgia.txt,1}, {i.txt,1}, {is.txt,1}, {my.txt,1}, {name.txt,1}, {not.txt,1}, {shruti.txt,1}, {study.txt,1}, {tech.txt,1}]

### Reduce tasks
Number of files assigned to each worker : 10/2 = 5

Worker1:
    Assigned intermediate files : at.txt, georgia.txt, i.txt, is.txt, my.txt
    Output file number : 0
    For each intermediate file, user reduce function is called as reduce(at, [1])

    Output file created : 0.txt
    at 1
    georgia 1
    i 1
    is 2
    my 2

Worker2: 
    Assigned intermediate files : name.txt, not.txt, shruti.txt, study.txt, tech.txt
    Output file number : 1

    Output file created : 1.txt
    name 2
    not 1
    shruti 2
    study 1
    tech 1