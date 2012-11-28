require( "luacurl" )
require("json")
domain='api.weibo.com'
version='2'
--dofile('table.lua')
token = ""
 c = curl.new() -- curl.new not found
function weiboapiget( apiname,token,param )
    if not param then
        param=''
    end
	
	local t={}
	c:setopt( curl.OPT_WRITEFUNCTION, function ( stream, buffer )
		table.insert(t,buffer)
		return string.len( buffer )
	end);

	--c:setopt( curl.OPT_WRITEDATA, io.open( "lua-5.0.2.tar.gz", "wb" ) )

	c:setopt( curl.OPT_PROGRESSFUNCTION, function ( _, dltotal, dlnow, uptotal, upnow )
		print( dltotal, dlnow, uptotal, upnow )
	end )

	c:setopt( curl.OPT_NOPROGRESS, false )
    ---------------------------------------------
    c:setopt( curl.OPT_SSLENGINE_DEFAULT, false )
    c:setopt( curl.OPT_SSL_VERIFYPEER, false )
    c:setopt( curl.OPT_SSL_VERIFYHOST, 0 )
    ---------------------------------------------

	c:setopt( curl.OPT_HTTPHEADER, "Connection: Keep-Alive", "Accept-Language: en-us",
	'Authorization:'..' OAuth2 '..token
	 )

	c:setopt( curl.OPT_URL, 'https://'..domain..'/'..version..'/'..apiname..'.json?'..param )
	c:setopt( curl.OPT_CONNECTTIMEOUT, 15 )
	c:perform()
	--c:close()
    --for k,v in pairs(t) do
   -- print (k,v)
   -- end
	--print(string.join("",t))--error
    --table.concat(table_of_strings, separator)
    return table.concat(t,"")
end
function getTLName()
    return tl[index]['user']['screen_name']
end

function getTLText()
    local txt = tl[index]['text']
    if tl[index]['retweeted_status'] then
        txt = txt..'|转|'..tl[index]['retweeted_status']['user']['screen_name']..' : '..tl[index]['retweeted_status']['text']
    end
    return txt
end
function openUrl(  )
    local mid = tl[index]['mid']
end
function OnHotKey(keyid)
    if not tl and type(tl) ~='table' then
        return
    end
    if keyid == 2012 then
        if(index+1 <= #tl) then
            index = index+1
        else
            page = page+1
            newpg = json.decode(weiboapiget('statuses/home_timeline',token,'page='..page))['statuses']
			if newpg and #newpg==0 then
				return
			else
				--tl = tl+newpg
                for k,v in pairs(newpg) do
                    table.insert(tl,v)
                end
				index = index + 1
            end
        end
		pos = 1
		show_text(getTLName().." : ".. getTLText())
        
    elseif keyid == 2013 then
        if(index>1)then
			index = index-1
		else
			return
        end
		pos = 1
		show_text(getTLName().." : ".. getTLText())
    elseif keyid == 2014 then
    	tl = json.decode(weiboapiget('statuses/home_timeline',token,'page='..page))['statuses']
		index=1
		pos = 1
		page = 1
		show_text(getTLName().." : ".. getTLText())
    elseif keyid == 2015 then
        restore_text()
    elseif keyid == 2016 then
        restore_text()
    elseif keyid == 2017 then
        if (pos+1)<u8charcount(getTLText()) then
            pos = pos + 1
            show_text(getTLName().." : ".. u8sub(getTLText(),pos))
        end
    elseif keyid == 2018 then
        if(pos>1)then
            pos = pos - 1
            show_text(getTLName().." : ".. u8sub(getTLText(),pos))
        end
    elseif keyid == 2019 then
        local uid = tl[index]['user']['id']
        local mid = tl[index]['mid']
        os.execute ("start http://microbar.rmbback.com/wb?uid="..uid.."^&mid="..mid)
    elseif keyid == 2020 then
    elseif keyid == 2021 then
    elseif keyid == 2022 then
    end
end
--os.execute ("start http://www.weibo.com")
--return show_text("1234qwer")

--s=weiboapiget('statuses/home_timeline',token,'page='..1)
--print("s:"..string.sub(s,1,100))
--s2=weiboapiget('statuses/home_timeline',token,'page='..2)
--print("s2:"..string.sub(s2,1,100))
--tl = json.decode(s)['statuses']
--print(TableToStr(timeline))

index=1
page = 1
pos = 1
--show_text(tl[1]['user']['screen_name']..":"..tl[1]['text'])
--print (tl[1]['user']['screen_name']..":"..tl[1]['text'])



function u8sub(s,pos)
    --return string.sub(s,pos)

    local n = 1
    for i = 1,#s,1 do
        if string.byte(s,n) <128 --10000000 1 byte char
            then
            n=n+1
            pos = pos -1
        elseif string.byte(s,n) >= 192  --11000000
            and string.byte(s,n) <= 223 --11011111 2 bytes char
            then
            n = n+2
            pos = pos -1
        elseif  string.byte(s,n) >= 224  --11100000
            and string.byte(s,n) <= 239 --11101111 3 bytes char
            then
            n = n+2
            pos = pos -1
        elseif  string.byte(s,n) >= 240  --11110000
            and string.byte(s,n) <= 247 --11110111 4 bytes char
            then
            n = n+3
            pos = pos -1
        else
            n=n+1
        end
        
        
        if pos <= 0 then 
            break
        end
    end
    print(n,pos,s)
    return string.sub(s,n-2)
end
function  u8charcount( s )
    --return #s
    local n = 0
    if #s <=1 then
        return #s
    end
    for i = 1,#s,1 do
        if string.byte(s,i) <128 --10000000 1 byte char
            then
            n=n+1

        elseif string.byte(s,i) >= 192  --11000000
            and string.byte(s,i) <= 223 --11011111 2 bytes char
            then
            n = n+1

        elseif  string.byte(s,i) >= 224  --11100000
            and string.byte(s,i) <= 239 --11101111 3 bytes char
            then
            n = n+1

        elseif  string.byte(s,i) >= 240  --11110000
            and string.byte(s,i) <= 247 --11110111 4 bytes char
            then
            n = n+1
        end
    end
    return n
end
--s= "我的地盘我做主1234343"

--print (u8charcount(s))
--for k = 1, u8charcount(s),1 do
--print(k,u8sub(s,1))
--end
