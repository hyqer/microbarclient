require( "luacurl" )
local domain='api.weibo.com'
local version='2'

function weiboapiget( url,token,param )
	local c = curl.new() -- curl.new not found
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

	c:setopt( curl.OPT_HTTPHEADER, "Connection: Keep-Alive", "Accept-Language: en-us"--,
	--'Authorization:'.'OAuth2 %s'.token
	 )

	c:setopt( curl.OPT_URL, "https://github.com/ignacio/LuaOAuth" )
	c:setopt( curl.OPT_CONNECTTIMEOUT, 15 )
	c:perform()
	c:close()
	print(string.join("",t))
end
