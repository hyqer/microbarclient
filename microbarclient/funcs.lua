require( "luacurl" )
require("json")
domain='api.weibo.com'
version='2'
dofile('table.lua')
token = "2.00lyA8HB0J6NEw9197666ad20IG7bl"
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

function OnHotKey(keyid)
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
		show_text(tl[index]['user']['screen_name']..":".. tl[index]['text'])
        
    elseif keyid == 2013 then
        if(index>1)then
			index = index-1
		else
			return
        end
		pos = 0
		show_text(tl[index]['user']['screen_name']..":".. tl[index]['text'])
    elseif keyid == 2014 then
    	tl = json.decode(weiboapiget('statuses/home_timeline',token,'page='..page))['statuses']
		index=1
		pos = 1
		page = 1
		show_text(tl[index]['user']['screen_name']..":".. tl[index]['text'])
    elseif keyid == 2015 then
    elseif keyid == 2016 then
    elseif keyid == 2017 then
    elseif keyid == 2018 then
    elseif keyid == 2019 then
    elseif keyid == 2020 then
    elseif keyid == 2021 then
    elseif keyid == 2022 then
    end
end
--os.execute ("start http://www.weibo.com")
--return show_text("1234qwer")

s=weiboapiget('statuses/home_timeline',token,'page='..1)
print("s:"..string.sub(s,1,100))
--s2=weiboapiget('statuses/home_timeline',token,'page='..2)
--print("s2:"..string.sub(s2,1,100))
tl = json.decode(s)['statuses']
--print(TableToStr(timeline))

index=1
hLast = 0
sLast = 0
page = 1
pos = 1
--show_text(tl[1]['user']['screen_name']..":"..tl[1]['text'])
--print (tl[1]['user']['screen_name']..":"..tl[1]['text'])



-- function u8sub(s,pos)
    -- local n=1
    -- for i in 1,pos do
        -- if string 
    -- end
    
    -- return string.sub(s,n)
-- end