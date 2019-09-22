#!/usr/bin/env python3

#SBATCH --job-name=openmp_scale     # Job name
#SBATCH --nodes=1                   # Use one node
#SBATCH --exclusive
#SBATCH --time=04:00:00             # Time limit hrs:min:sec

import os
import subprocess
import datetime as dt
import time
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties
from collections import defaultdict, deque



matplotlib.use('PDF')
plt.ioff()

n_threads = []
total_time = []
ops_time = []



##############################################################
def slurm_header():
    now = dt.datetime.now()
    print(" " + 79*"-")
    print("| Started:       {}".format(now.strftime("%Y-%m-%d %H:%M:%S")))
    print("| Case name:     {}".format(os.environ.get('SLURM_JOB_NAME')))
    print("| Job ID:        {}".format(os.environ.get('SLURM_JOBID')))
    print("| Nodes:         {}".format(os.environ.get('SLURM_JOB_NODELIST')))
    print("| Working Dir:   {}".format(os.environ.get('SLURM_SUBMIT_DIR')))
    print("| # of Procs:    {}".format(os.environ.get('SLURM_NPROCS')))
    print("| # of Nodes:    {}".format(os.environ.get('SLURM_NNODES')))
    print("| # Tasks/Node:  {}".format(os.environ.get('SLURM_TASKS_PER_NODE')))
    print("|")
    print("| SLURM_JOB_CPUS_PER_NODE={}".format(os.environ.get('SLURM_JOB_CPUS_PER_NODE')))

    # print("| SLURM_ARRAY_TASK_ID={}".format(os.environ.get('SLURM_ARRAY_TASK_ID')))
    # print("| SLURM_ARRAY_TASK_COUNT={}".format(os.environ.get('SLURM_ARRAY_TASK_COUNT')))
    # print("| SLURM_ARRAY_TASK_MIN={}".format(os.environ.get('SLURM_ARRAY_TASK_MIN')))
    # print("| SLURM_ARRAY_TASK_MAX={}".format(os.environ.get('SLURM_ARRAY_TASK_MAX')))
    print(" " + 79*"-")
    return



##############################################################
def slurm_footer():
    now = dt.datetime.now()
    print(" " + 79*"-")
    print("| Finished:      {}".format(now.strftime("%Y-%m-%d %H:%M:%S")))
    print(" " + 79*"-")
    return



##############################################################
def run_tool(tool, matsize, nt=1):

    commands = []
    commands.append('./{} {} {}'.format(tool, matsize, nt))



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
        print('{:.3f}, {:.3f}'.format(tt,ot))

        #print('-'*80)
        #print('')


    print('='*80)
    print('average runtime, nt={}, tot={:.3f}, ops={:.3f}'.format(nt,
                                                                  np.mean(tot_time_t),
                                                                  np.mean(ops_time_t)))
    print('variance, tot={:.3f}, ops={:.3f}'.format(np.var(tot_time_t),
                                                    np.var(ops_time_t)))
    print('='*80)

    return (nt, np.mean(tot_time_t), np.mean(ops_time_t))





##############################################################
if __name__ == "__main__":

    prefix     = ''
    job_name   = os.environ.get('SLURM_JOB_NAME', None)
    job_id     = os.environ.get('SLURM_JOBID',    None)
    maxthreads = int(os.environ.get('SLURM_JOB_CPUS_PER_NODE', 56))

    if job_name: slurm_header()
    if job_id:
        try:
            prefix='out-{}/'.format(job_id)
            os.mkdir(prefix)
        except:
            pass


    print('Running on maxthreads={}'.format(maxthreads))

    f=open(prefix + 'results.csv', "w+")
    f.write('NT, Total, Ops, s(Tot), s(Ops)\n')

    #tools = [ 'matprod-r4', 'matprod-r8', 'lu', 'ldlt', 'gs_openmp' ]
    tools = [ 'matprod-r4', 'matprod-r8', 'lu' ]

    for tool in tools:


        for pow in range(12,15):


            n_threads = []
            total_time = []
            ops_time = []
            sp1=[]
            sp2=[]

            matsize = 2**pow
            print('N=2^{}={}'.format(pow,matsize))

            f.flush()
            f.write('# {}, N=2^{}={}\n'.format(tool,pow,matsize))


            nt=1
            inc=2
            while nt <= maxthreads:
                vals = run_tool(tool,matsize, nt)
                n_threads.append(vals[0])
                total_time.append(vals[1])
                ops_time.append(vals[2])
                f.write('{}, \t{:.5f}, \t{:.5f}, \t{:.2f}, \t{:.2f}\n'.format(vals[0], vals[1], vals[2],
                                                                              total_time[0]/vals[1],
                                                                              ops_time[0]/vals[2]))
                inc = 2 if nt<=10 else 4
                nt += 1 if nt==1 else inc



            print('*'*80)
            print('*'*80)

            for s in total_time: sp1.append(total_time[0]/s)
            for s in ops_time:   sp2.append(ops_time[0]/s)

            plt.clf()
            plt.title('{} OpenMP Speedup, N={} Matrix'.format(tool, matsize))
            plt.grid(True,axis='both',color='grey',linestyle=':', linewidth=0.5)
            plt.plot(n_threads, n_threads, linewidth=3, color='black')
            plt.plot(n_threads, sp1, '--', linewidth=2, color='red')
            plt.plot(n_threads, sp2,       linewidth=3, color='red')
            plt.xlabel('# threads')
            plt.ylabel('Speedup', color='red')
            plt.xlim([1, maxthreads])
            plt.ylim([1, maxthreads])


            ax_y2 = plt.twinx()
            ax_y2.plot(n_threads, total_time, '--', linewidth=1, color='grey')
            ax_y2.plot(n_threads, ops_time,         linewidth=2, color='grey')
            ax_y2.set_ylabel('Runtime (sec)', color='grey')
            ax_y2.set_yscale('log')
            ymin=10**np.floor(np.log10(np.amin(ops_time)))
            ymax=10**np.ceil (np.log10(np.amax(total_time)))
            ax_y2.set_ylim([ymin, ymax])

            plt.savefig(prefix + 'speedup_{}_N{}.pdf'.format(tool, matsize))
        #plt.show()

    f.close()
    if job_name: slurm_footer()

    exit
