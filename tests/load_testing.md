ab -n 10000 -c 200 -k -H 'X-Token: y0_AgAAAAA_Q5WvAAvG9wAAAAEFOVs0AADgDluj95BJurQ1Y0iUf1NgF7u8gQ' http://localhost/api/v3/events
This is ApacheBench, Version 2.3 <$Revision: 1903618 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking localhost (be patient)
Completed 1000 requests
Completed 2000 requests
Completed 3000 requests
Completed 4000 requests
Completed 5000 requests
Completed 6000 requests
Completed 7000 requests
Completed 8000 requests
Completed 9000 requests
Completed 10000 requests
Finished 10000 requests


Server Software:        nginx/1.27.2
Server Hostname:        localhost
Server Port:            80

Document Path:          /api/v3/events
Document Length:        53 bytes

Concurrency Level:      200
Time taken for tests:   6.906 seconds
Complete requests:      10000
Failed requests:        22
(Connect: 0, Receive: 0, Length: 22, Exceptions: 0)
Non-2xx responses:      22
Keep-Alive requests:    10000
Total transferred:      5395908 bytes
HTML transferred:       528871 bytes
Requests per second:    1447.95 [#/sec] (mean)
Time per request:       138.126 [ms] (mean)
Time per request:       0.691 [ms] (mean, across all concurrent requests)
Transfer rate:          762.99 [Kbytes/sec] received

Connection Times (ms)
min  mean[+/-sd] median   max
Connect:        0    0   0.6      0       6
Processing:     5  137 186.5     51    1484
Waiting:        5  137 186.5     51    1484
Total:          5  137 186.5     51    1484

Percentage of the requests served within a certain time (ms)
50%     51
66%    102
75%    176
80%    237
90%    394
95%    564
98%    764
99%    827
100%   1484 (longest request)

# Вывод

Сервис поднятый на ноуте держит порядка 1200РПС.
Этот результат получен после большого количества запусков ab.
