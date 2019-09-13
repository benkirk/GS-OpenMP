#!/usr/bin/env python

import subprocess
import datetime as dt
import time
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties
from collections import defaultdict, deque



n_threads = []
total_time = []
ops_time = []


##############################################################
def run_gs(matsize, nt=1):

    commands = []
    commands.append('./gs_openmp {} {}'.format(matsize, nt))



    tot_time_t = []
    ops_time_t = []

    maxit=5
    for iter in range(0,maxit):
        #print('-'*80)

        ot = None
        tt = None

        for command in commands:
            print(' instance {} of {}, {}'.format(iter+1,maxit, command),end=', ')
            process = subprocess.Popen(command,
                                       shell=True,
                                       stdout=subprocess.PIPE)

            processed_vars = set()

            while True:
                output = process.stdout.readline().decode('utf-8')

                if output == '' and process.poll() is not None: break

                if output:
                    output=output.rstrip('\n')
                    #print(output)

                    if 'max_threads:' in output:
                        assert nt == int(output.split(':')[1])

                    elif 'Total time:' in output:
                        tt = float(output.split(':')[1])
                        tot_time_t.append(tt)

                    elif 'Operations time:' in output:
                        ot = float(output.split(':')[1])
                        ops_time_t.append(ot)

                    else:
                        print('{}'.format(output),end=', ')



            rc = process.poll()
        print('{}, {}'.format(tt,ot))

        #print('-'*80)
        #print('')


    print('='*80)
    print('average runtime, nt={}, tot={}, ops={}'.format(nt,
                                                          np.mean(tot_time_t),
                                                          np.mean(ops_time_t)))
    print('variance, tot={}, ops={}'.format(np.var(tot_time_t),
                                            np.var(ops_time_t)))
    print('='*80)

    return (nt, np.mean(tot_time_t), np.mean(ops_time_t))





##############################################################
if __name__ == "__main__":


    for pow in range(12,15):

        n_threads = []
        total_time = []
        ops_time = []
        sp1=[]
        sp2=[]

        matsize = 2**pow
        print('N = 2^{} = {}'.format(pow,matsize))

        vals = run_gs(matsize, 1)
        n_threads.append(vals[0])
        total_time.append(vals[1])
        ops_time.append(vals[2])

        nt=2
        mt=56
        while nt <= mt:
            vals = run_gs(matsize, nt)
            n_threads.append(vals[0])
            total_time.append(vals[1])
            ops_time.append(vals[2])
            nt += 2



        print('*'*80)
        print('*'*80)
        plt.plot(n_threads, total_time)
        plt.plot(n_threads, ops_time)
        plt.xlabel('# threads')
        plt.ylabel('Runtime (sec)')


        for s in total_time: sp1.append(total_time[0]/s)
        for s in ops_time:   sp2.append(ops_time[0]/s)

        ax_y2 = plt.twinx()
        ax_y2.plot(n_threads, n_threads)
        ax_y2.plot(n_threads, sp1)
        ax_y2.plot(n_threads, sp2)
        plt.ylabel('Speedup')
        plt.show()


    print('Bye!')

    exit
