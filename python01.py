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
    
#使用python读取参数文件
a.txt文件的内容
[config]
host=xx
user=yy

import configparser
cf=configparser.ConfigParser()
cf.read("a.txt")
secs=cf.sections()
print(secs)
options=cf.options("config")
print(options)
items=cf.items("config")
print(items)
host=cf.get("config","host")
print(host)


#接收电子邮件

import poplib
import base64
from email.parser import Parser
from email.header import decode_header
from email.utils import parseaddr
 
def get_email_content():
	useraccount = 'lglgang@126.com'
	password = 'cqmyg_ysdss126'
 
	# 邮件服务器地址,以下为网易邮箱
	pop3_server = 'pop.126.com'
 
	# 开始连接到服务器
	server = poplib.POP3(pop3_server)
 
	# 打开或者关闭调试信息，为打开，会在控制台打印客户端与服务器的交互信息
	server.set_debuglevel(1)
 
	# 打印POP3服务器的欢迎文字，验证是否正确连接到了邮件服务器
	print(server.getwelcome().decode('utf8'))
 
	# 开始进行身份验证
	server.user(useraccount)
	server.pass_(password)
 
	# 返回邮件总数目和占用服务器的空间大小（字节数）， 通过stat()方法即可
	email_num, email_size = server.stat()
	print("消息的数量: {0}, 消息的总大小: {1}".format(email_num, email_size))
 
	# 使用list()返回所有邮件的编号，默认为字节类型的串
	rsp, msg_list, rsp_siz = server.list()
	print("服务器的响应: {0},\n消息列表： {1},\n返回消息的大小： {2}".format(rsp, msg_list, rsp_siz))
 
	print('邮件总数： {}'.format(len(msg_list)))
 
	# 下面单纯获取最新的一封邮件
	total_mail_numbers = len(msg_list)
	rsp, msglines, msgsiz = server.retr(total_mail_numbers)
	#print("服务器的响应: {0},\n原始邮件内容： {1},\n该封邮件所占字节大小： {2}".format(rsp, msglines, msgsiz))
 
	msg_content = b'\r\n'.join(msglines).decode('gbk')
 
	msg = Parser().parsestr(text=msg_content)
	email = Parser().parsestr(msg_content)
	print('解码后的邮件信息:\n{}'.format(msg))
 
	# 关闭与服务器的连接，释放资源
	server.close()
 
	return email
msgs=get_email_content()

print("================")
print(msgs)
msg1=msgs['Subject']
print("================")
print(msg1)
msg2=msg1.split("?")
print(msg2[3])
bbs = str(base64.b64decode(msg2[3]), "utf-8")
print(bbs)


    
