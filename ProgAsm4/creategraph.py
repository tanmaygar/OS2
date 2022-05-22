import matplotlib.pyplot as plt
import numpy as np

num_process = [10, 20, 30, 40, 50]

average_waiting_CAS_10 = np.average([2.71, 2.65, 2.76, 2.69, 2.73])
average_waiting_CAS_20 = np.average([7.125, 6.825, 6.945, 7.105, 7.05])
average_waiting_CAS_30 = np.average([11.15, 10.94, 11.0033, 11.0367, 10.9133])
average_waiting_CAS_40 = np.average([15.2475, 14.74, 15.095, 15.285, 14.925])
average_waiting_CAS_50 = np.average([19.384, 19.01, 19.316, 19.03, 18.966])

average_waiting_TAS_10 = (2.7 + 2.7 + 2.68 + 2.68 + 2.65) / 5
average_waiting_TAS_20 = np.average([6.915, 7.05, 7.11, 7.175, 7.015])
average_waiting_TAS_30 = np.average([10.7133, 10.9167, 11.12, 11.0633, 10.8633])
average_waiting_TAS_40 = np.average([15.1175, 14.7525, 15.0975, 15.0575, 14.9825])
average_waiting_TAS_50 = np.average([19.616, 19.522, 19.648, 19.218, 18.902])

average_waiting_BCAS_10 = 2.99
average_waiting_BCAS_20 = np.average([7.525, 7.525, 7.615, 7.525, 7.62])
average_waiting_BCAS_30 = np.average([12.1, 12.0967, 12.1733, 12.1, 12.17])
average_waiting_BCAS_40 = np.average([16.685, 16.785, 16.7825, 16.7825, 16.7875])
average_waiting_BCAS_50 = np.average([21.468, 21.47, 21.466, 21.47, 21.478])

worst_waiting_CAS_10 = np.average([28, 23, 18, 22, 19])
worst_waiting_CAS_20 = np.average([43, 47, 61, 42, 48])
worst_waiting_CAS_30 = np.average([68, 84, 73, 79, 96])
worst_waiting_CAS_40 = np.average([117, 119, 110, 112, 131])
worst_waiting_CAS_50 = np.average([125, 158, 150, 170, 161])

worst_waiting_TAS_10 = (21 + 21 + 23 + 20 + 21) / 5
worst_waiting_TAS_20 = np.average([61, 48, 53, 43, 43])
worst_waiting_TAS_30 = np.average([127, 105, 78, 78, 88])
worst_waiting_TAS_40 = np.average([102, 136, 124, 100, 111])
worst_waiting_TAS_50 = np.average([129, 145, 148, 129, 175])

worst_waiting_BCAS_10 = 18
worst_waiting_BCAS_20 = 38
worst_waiting_BCAS_30 = np.average([58, 58, 58, 58, 58])
worst_waiting_BCAS_40 = np.average([78, 79, 78, 77, 79])
worst_waiting_BCAS_50 = np.average([98, 99, 98, 98, 98])


cas_average_val = [average_waiting_CAS_10, average_waiting_CAS_20, average_waiting_CAS_30, average_waiting_CAS_40, average_waiting_CAS_50]
cas_worst_val = [worst_waiting_CAS_10, worst_waiting_CAS_20, worst_waiting_CAS_30, worst_waiting_CAS_40, worst_waiting_CAS_50]
tas_average_val = [average_waiting_TAS_10, average_waiting_TAS_20, average_waiting_TAS_30, average_waiting_TAS_40, average_waiting_TAS_50]
tas_worst_val = [worst_waiting_TAS_10, worst_waiting_TAS_20, worst_waiting_TAS_30, worst_waiting_TAS_40, worst_waiting_TAS_50]
b_average_val = [average_waiting_BCAS_10, average_waiting_BCAS_20, average_waiting_BCAS_30, average_waiting_BCAS_40, average_waiting_BCAS_50]
b_worst_val = [worst_waiting_BCAS_10, worst_waiting_BCAS_20, worst_waiting_BCAS_30, worst_waiting_BCAS_40, worst_waiting_BCAS_50]

plt.figure()
plt.plot(num_process, cas_average_val, 'r', label='CAS')
plt.plot(num_process, tas_average_val, 'g', label='TAS')
plt.plot(num_process, b_average_val, 'b', label='BCAS')
plt.xlabel('Number of Processes')
plt.ylabel('Average Waiting Time')
plt.title('Average Waiting Time vs Number of Processes')
#plt.yticks(np.arange(0, 25, 2))
plt.xticks(num_process)
plt.grid(True)
plt.legend()
plt.show()

plt.figure()
plt.plot(num_process, cas_worst_val, 'r', label='CAS')
plt.plot(num_process, tas_worst_val, 'g', label='TAS')
plt.plot(num_process, b_worst_val, 'b', label='BCAS')
plt.xlabel('Number of Processes')
plt.ylabel('Worst Waiting Time')
plt.title('Worst Waiting Time vs Number of Processes')
plt.xticks(num_process)
plt.grid(True)
plt.legend()
plt.show()
