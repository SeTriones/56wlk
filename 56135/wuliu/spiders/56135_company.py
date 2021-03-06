#-*- coding: UTF-8 -*-   
import sys
reload(sys)

sys.setdefaultencoding('utf-8') 
#----->>决文件里面不能写汉字的编码问题

from scrapy.selector import HtmlXPathSelector
from scrapy.contrib.linkextractors.sgml import SgmlLinkExtractor
from scrapy.http import Request
from scrapy.contrib.spiders import CrawlSpider, Rule
import time
from wuliu.items import A56135CompanyItem

import re


class ChinaWuTongLine201Spider(CrawlSpider):
    name = '56135_company'

    allowed_domains = ['56135.com']
    start_urls=[]
    #for no in range(333400,5000000):
    #    start_urls.append('http://www.chinawutong.com/201/%d.html'%no)
    
    start_urls = ['http://56135.com/56135/company/companylist-adv-1.html']
    #start_urls = ['http://56135.com/56135/company/memberbrief/C443160.html']
    #start_urls = ['http://56135.com/56135/company/memberbrief/C443182.html']
    #rules = (
            #Rule(SgmlLinkExtractor(allow=r'/202/[\d]+\.html'), callback='parse_item', follow=True),
    #)
    def start_requests(self):
	items = []
    	for url in self.start_urls:
           req = Request(url, cookies={'ASP.NET_SessionId':'coxgzw45tyqpyifvf4eziwul'})
	   items.append(req)
	return items

    def parse(self, response):
        hxs = HtmlXPathSelector(response)
        items = []
        page_str = hxs.select('.//*[@id="MainMember1_Newpagebar1_l_pagebar"]/div/a[9]/text()').extract()[0]
        p = re.compile(r'.+?(\d+)$')
        total_page_num = p.findall(page_str)[0]
	#print total_page_num

        for page_num in range(int(total_page_num), 2188, -1):
            req = Request(
			url = 'http://56135.com/56135/company/companylist-adv-%d.html'%page_num,
			#cookies={'ASP.NET_SessionId':'skyar1yam15adu55avnhajv3'},
			callback = self.parse_hub,
                    )
            items.append(req)

        return items

    def parse_hub(self, response):
	#print response.url
	#print response.body
       #pre_url = "http://dx88.56888.net/Line/"

       hxs = HtmlXPathSelector(response)
       links = hxs.select('.//*[@id="mainBox"]/div[2]/div[1]/div[3]/div[1]/div[1]/div/div[2]/a[2]/@href').extract()
       items = []
       for link in links:
           p = re.compile(r'.+?memberno=(.*)$')
           token = p.findall(link)[0]
           req = Request(
                   url = "http://56135.com/56135/company/memberbrief/%s.html"%token,
                   callback = self.parse_detail,
                   )
           items.append(req)
       return items


    def parse_detail(self, response):
	#print response.body
        items = []
        item = A56135CompanyItem()
            
        hxs = HtmlXPathSelector(response)

        item['url'] = response.url  

	contact_name    = hxs.select('.//*[@id="l_memberlink"]/text()[1]').extract() #联系人：陈聪女士
        if len(contact_name) == 0:
	     contact_name[0:0] = ['']
        if len(contact_name[0].strip()) == 0:
             item['contact_name'] = ''
        else:
             item['contact_name'] = contact_name[0].strip()
             
        tel = hxs.select('.//*[@id="l_memberlink"]/font[1]/b/text()').extract()
        if len(tel) == 0:
	     tel[0:0] = ['']
	if len(tel[0].strip()) == 0:
             item['tel'] = ''
        else:
             item['tel'] = tel[0].strip()
             
        phone_contact = hxs.select('.//*[@id="l_memberlink"]/font[2]/b/text()').extract()
        if len(phone_contact) == 0:
	     phone_contact[0:0] = ['']
        if len(phone_contact[0].strip()) == 0:
             item['phone_contact'] = ''
        else:
             item['phone_contact'] = phone_contact[0].strip()

	mail = hxs.select('.//*[@id="l_memberlink"]/a[1]/text()').extract()
        if len(mail) == 0:
	     mail[0:0] = ['']
        if len(mail[0].strip()) == 0:
             item['mail'] = ''
        else:
             item['mail'] = mail[0].strip()

	im =  hxs.select('.//*[@id="l_memberlink"]/text()[7]').extract() #QQ:23432
        if len(im) == 0:
	     im[0:0] = ['']
        if len(im[0].strip()) == 0:
             item['im'] = ''
        else:
             item['im'] = im[0].strip()

        company_name = hxs.select('.//*[@id="CompanyName1_l_no"]/text()').extract()
        if len(company_name) == 0:
	     company_name[0:0] = ['']
        if len(company_name[0].strip()) == 0:
             item['company_name'] = ''
        else:
             item['company_name'] = company_name[0].strip()

	addr = hxs.select('.//*[@id="l_addr"]/text()').extract()
        if len(addr) == 0:
	     addr[0:0] = ['']
        if len(addr[0].strip()) == 0:
             item['addr'] = ''
        else:
             item['addr'] = addr[0].strip()
	
        remark    = hxs.select('.//*[@id="l_introduce"]//text()').extract()
        if len(remark) == 0:
	     remark[0:0] = ['']
        if len(remark[0].strip()) == 0:
             item['remark'] = ''
        else:
             item['remark'] = "".join(remark).strip()
                
        items.append(item)

        return items

