import redis
import datetime
import time

pool = redis.StrictRedis(host='127.0.0.1', port=6379, db=0)
pool.set("pyset","pysetdata")

counfile=open("counfile.txt","w")

while True:
    now = datetime.datetime.now()
    filename="jiludir/"+now.strftime("%Y-%m-%d %H:%M:%S")
    ostream=open(filename,"w")
    for idx in range(18000):
        data=pool.get("newrobot")
        strdata=str(data,'utf-8')
        coun=str.count(strdata,"motionSpeed\":{}")
        if(coun):
            coundata='%d' %coun
            counfile.write(strdata+'\n')
            counfile.write(coundata+'\n')
        ostream.write(strdata+'\n')
        time.sleep(0.2)
    ostream.close()

