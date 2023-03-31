from datetime import date, datetime
from time import sleep
from apscheduler.schedulers.background import BackgroundScheduler
from apscheduler.schedulers.blocking import BlockingScheduler
from apscheduler.triggers.cron import CronTrigger
from blinker import signal
from multiprocessing import Pipe
import os

class TestScheduler(object):

    def __init__(self) -> None:
        self.__sched = BlockingScheduler()
        self.__trigger = CronTrigger(
            day_of_week='mon-fri',
            hour='6-18',
            minute='0-59',
            second='0,10,30,40,50')
        self.__pip_r__,self.__pip_w__ = os.pipe()

    def __my_job(self, text) -> None:
        print(f'{text} , current time is {datetime.now()}')

    def add_job(self) ->None:
        self.__sched.add_job(func=TestScheduler.__my_job, trigger=self.__trigger,args=[self, 'text'])

    def start(self):
        self.__sched.start()

    def stop(self):
        self.__sched.shutdown()


if __name__ == '__main__':
    t = TestScheduler()
    t.add_job()
    t.start()