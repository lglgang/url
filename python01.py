#this is my python code 
#这个是读取编码器的代码，使用树莓派
from ctypes import *
encoder=cdll.LoadLibrary("/home/pi/./ls7366M0.so")
encoder.init_encoder2()
while 1:
    xx=encoder.get_counter2()
    print(xx)
#通过python发送邮件的代码
import smtplib
from email.mime.text import MIMEText
from email.header import Header
 
# 第三方 SMTP 服务
mail_host="smtp.126.com"  #设置服务器
mail_user="username"    #用户名
mail_pass="password"   #口令 
 
 
sender = 'lglgang@126.com'
receivers = ['lglgang@126.com']  # 接收邮件，可设置为你的QQ邮箱或者其他邮箱
 
message = MIMEText('aaaaa', 'plain', 'utf-8')
message['From'] = "ligang<lglgang@126.com>"
message['To'] =  "ligang<lglgang@126.com>"
 
subject = '你好'
message['Subject'] = Header(subject, 'utf-8')

try:
    smtpObj = smtplib.SMTP() 
    smtpObj.connect(mail_host, 25)    # 25 为 SMTP 端口号
    smtpObj.login(mail_user,mail_pass)  
    smtpObj.sendmail(sender, receivers, message.as_string())
    print("邮件发送成功")
except smtplib.SMTPException:
    print("Error: 无法发送邮件")
    
    
    
