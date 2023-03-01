from datetime import date, datetime
from time import sleep
from apscheduler.schedulers.background import BackgroundScheduler
from apscheduler.schedulers.blocking import BlockingScheduler
from apscheduler.triggers.cron import CronTrigger
from apscheduler.job import Job


class TestScheduler(object):

    def __init__(self) -> None:
        self.__sched = BlockingScheduler()
        self.__trigger = CronTrigger(
            month='1-12',
            day_of_week='mon-fri',
            hour='6-23',
            minute='0-59',
            second='0-59/2',
            timezone="Asia/Shanghai")

    def my_job(self, text) -> None:
        print(f'{text} , current time is {datetime.now()}')

    def add_job(self) -> None:
        _job: Job = self.__sched.add_job(func=TestScheduler.my_job,
                                         trigger=self.__trigger, args=[self, 'text'])
        print(_job.id)

    def start(self) -> None:
        self.__sched.start()


if __name__ == '__main__':
    t = TestScheduler()
    t.add_job()
    t.start()
