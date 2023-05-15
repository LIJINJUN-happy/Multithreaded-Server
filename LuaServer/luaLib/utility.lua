UTILITY = {}

--ÇĞ¸î×Ö·û´® 
--Args£¨´ıÇĞ¸îµÄ×Ö·û´®, ÇĞ¸î·û£© 
--Return ±»ÇĞ¸î×Ö·ûµÄÁĞ±í
function UTILITY:Split(str, char)
    local ret = {}
    local beginIndex,endIndex = string.find(str, char)
    local resHead, resTail
    while (beginIndex or endIndex) do
        resHead, resTail = string.sub(str, 1, beginIndex-1), string.sub(str, endIndex+1, #str)
        --print("resHead === ", resHead, " resTail === ", resTail)
        if resHead and #resHead >= 1 then
          table.insert(ret, resHead)
        end
        str = resTail
        beginIndex,endIndex = string.find(str, char)
    end
    if resTail and #resTail >= 1 then
        table.insert(ret, resTail)
    end
    return ret
end

return UTILITY