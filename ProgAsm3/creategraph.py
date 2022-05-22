import matplotlib.pyplot as plt
import numpy as np

num_process = [20, 30, 40, 50, 60, 70, 80, 90, 100]

num_deadline_RMS = [2, 0, 4, 4, 0, 8, 2, 1, 0]
num_deadline_EDF = [0, 0, 4, 2, 0, 7, 1, 0, 0]

avg_wait_RMS = [12.5, 1.0667, 3.45, 4.84, 3.51667, 9.44286, 10.975, 7.88889, 4.59]
avg_wait_EDF = [6.5, 1.0667, 5.1, 7.12, 3.46667, 14.1429, 13.525, 8.25556, 4.46]

plt.figure()
plt.plot(num_process, num_deadline_RMS, 'r', label='RMS')
plt.plot(num_process, num_deadline_EDF, 'b', label='EDF')
plt.xlabel('Number of Processes')
plt.ylabel('Number of Deadlines Missed')
plt.title('Number of Deadlines Missed vs Number of Processes')
plt.legend()
plt.show()

plt.figure()
plt.plot(num_process, avg_wait_RMS, 'r', label='RMS')
plt.plot(num_process, avg_wait_EDF, 'b', label='EDF')
plt.xlabel('Number of Processes')
plt.ylabel('Average Wait Time')
plt.title('Average Wait Time vs Number of Processes')
plt.legend()
plt.show()