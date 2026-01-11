--[[
	Roblox Chat App Addon - Client Module
	Handles client-side chat UI and user interactions
]]

local ChatClient = {}

-- Initialize chat UI
function ChatClient:Init()
	print("[ChatClient] Initializing chat client...")
	-- TODO: Create chat GUI frame
	-- TODO: Set up input field for messages
	-- TODO: Create message display panel
	-- TODO: Connect to server events
end

-- Send message to server
function ChatClient:SendMessage(message: string): void
	if not self:ValidateInput(message) then
		warn("[ChatClient] Invalid message input")
		return
	end
	
	print("[ChatClient] Sending message: " .. message)
	-- TODO: Call server RemoteFunction to send message
end

-- Display message in chat
function ChatClient:DisplayMessage(player: string, message: string): void
	print("[ChatClient] " .. player .. ": " .. message)
	-- TODO: Update chat GUI with new message
end

-- Validate user input
function ChatClient:ValidateInput(message: string): boolean
	if type(message) ~= "string" then
		return false
	end
	
	if string.len(message) == 0 or string.len(message) > 1000 then
		return false
	end
	
	return true
end

-- Initialize on load
ChatClient:Init()

return ChatClient
