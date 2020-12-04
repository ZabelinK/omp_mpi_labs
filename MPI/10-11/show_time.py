import subprocess
import time
import matplotlib.pyplot as plt

command = 'mpiexec -n 1 ./bcast'
subprocess.call(command.split(' '))

max_process_number = 230

time_res_bcast = {}
for n in range(1, max_process_number, 10):
    command = 'mpiexec -n {} ./bcast'.format(n)
    start_time = time.time()
    subprocess.call(command.split(' '))
    res_time = time.time() - start_time
    print("Bcast time for {} : {}".format(n, res_time))
    time_res_bcast[n] = res_time

time_res_peer = {}
for n in range(1, max_process_number, 10):
    command = 'mpiexec -n {} ./peer'.format(n)
    start_time = time.time()
    subprocess.call(command.split(' '))
    res_time = time.time() - start_time
    print("Peers time for {} : {}".format(n, res_time))
    time_res_peer[n] = res_time

plt.plot(time_res_bcast.keys(), time_res_bcast.values(), label="MPI_Bcast()")
plt.plot(time_res_peer.keys(), time_res_peer.values(), label="MPI_Send()/MPI_Recv()")
plt.xlabel("Number of process")
plt.ylabel("Time, s")
plt.title("Comparasing MPI_Bcast and peer-to-peer communication")
plt.legend()
plt.show()