--
-- Created by IntelliJ IDEA.
-- User: alfredo
-- Date: 1/17/19
-- Time: 6:02 PM
-- To change this template use File | Settings | File Templates.
--

package.path = package.path .. ";/home/alfredo/CLionProjects/lua/tests/lua/includes/?.lua"
require "persistence"

receivers = {}
receivers[1] = {"john", "1548156150", "100", "BTS", false} -- 2019-01-22 11:22:30
receivers[2] = {"peter", "1550143350", "120", "BTS", false} -- 2019-02-14 11:22:30
receivers[3] = {"tamami", "1550661750", "110", "BTS" , false} -- 2019-02-20 11:22:30
receivers[4] = {"alfredo", "1551180150", "110", "BTS", false} -- 2019-02-26 11:22:30

storage_path = "/home/alfredo/CLionProjects/lua/tests/lua/contracts/receivers_db.lua"

check_database = persistence.load(storage_path)
if check_database == nil then
    persistence.store(storage_path, receivers)
end

restored = persistence.load(storage_path)
receivers = restored

payer = "ryan"
current_time = Bitshares:getCurrentTime()

for k,v in pairs(receivers) do
    if current_time >= tonumber(receivers[k][2]) then
        if not receivers[k][5] then
            Bitshares:transfer(payer, receivers[k][1], receivers[k][3], receivers[k][4], "")
            receivers[k][5] = true
            persistence.store(storage_path, receivers);
        end
    end
end

-- extra watch each block
block = Bitshares:getCurrentBlock()
print (block)
