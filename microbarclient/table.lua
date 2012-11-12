function  TableToStr(cTable)		--���ڽ�һ��lua tableת��Ϊlua string
	local str = '{'
	if cTable ~= nil and type(cTable) == 'table' then
		for k,v in pairs(cTable) do
			if type(k) == 'number' then			--�±������֣���Ҫ��Ӷ�Ӧ��
				str = str..'['..k..'] = '
			else
				if type(k) == 'string' then
					-- k = string.gsub( k, "\n", "\\n" )
					-- k = string.gsub(k,'"', '\\"' )
					-- k = string.gsub(k,"'", "\\'")
					str = str.."['"..k.."'] = "
				end
			end
			if type(v) == 'table'  then
				str = str..TableToStr(v)..','
			else
				if type(v) == 'string' then
					-- v = string.gsub( v, "\n", "\\n" )
					-- v = string.gsub(v,'"', '\\"' )
					-- v = string.gsub(v,"'", "\\'")
					str = str.."\'"..v.."\',"
				elseif type(v) == 'number' then
					str = str..v..","
				end
			end
		end
		if string.sub(str,string.len(str)-1) == ',' then	--ȥ�����һ�����ţ�������ٵ���������
			str = string.sub(str,1,string.len(str)-1)
		end
		str = str..' } '
		return str
	else
		--print("TableToStr cTable is error",cTable)
	end
end


function table_is_empty(t)  -- �жϱ��Ƿ�Ϊ��
	-- print("1")
	-- if type(t) == 'table' then
		-- print("2")
		-- return next( t ) == nil
	-- else
		-- print("3")
		-- return nil
	-- end
	--
	if type(t) == 'table' then
		local count = 0
		for k,v in pairs(t) do
			if v then
				return false
			end
		end
		
		if count == 0 then
			return true
		end
	else
		return nil
	end
end

function deepcopy(object) -- �����ݵ����
    local lookup_table = {} 
	local function _copy(object)
        if type(object) ~= "table" then
            return object
        elseif lookup_table[object] then
            return lookup_table[object]
        end
        local new_table = {}
        lookup_table[object] = new_table
        for index, value in pairs(object) do
            new_table[_copy(index)] = _copy(value)
        end
        return setmetatable(new_table, getmetatable(object))
    end
    return _copy(object)
end

function  TableToStr2(cTable)		--���ڽ�һ��lua tableת��Ϊlua string
	local str = '{'
	if cTable ~= nil and type(cTable) == 'table' then
		for k,v in pairs(cTable) do
			if type(k) == 'number' then			--�±������֣���Ҫ��Ӷ�Ӧ��
				str = str..'['..k..'] = '
			else
				if type(k) == 'string' then
					str = str..k.." = "
				end
			end
			if type(v) == 'table'  then
				str = str..TableToStr(v)..','
			else
				if type(v) == 'string' then
					str = str.."\'"..v.."\',"
				elseif type(v) == 'number' then
					str = str..v..","
				elseif type(v) == 'boolean' then
					str = str..v..","
				end
			end
		end
		if string.sub(str,string.len(str)-1) == ',' then	--ȥ�����һ�����ţ�������ٵ���������
			str = string.sub(str,1,string.len(str)-1)
		end
		str = str..' } '
		return str
	else
		--print("TableToStr cTable is error",cTable)
	end
end
