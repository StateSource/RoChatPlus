--[[
	Shared Type Definitions
	Common types used across the chat addon
]]

export type Message = {
	id: string,
	sender: string,
	content: string,
	timestamp: number,
}

export type ChatConfig = {
	maxMessageLength: number,
	messageRefreshRate: number,
	maxHistorySize: number,
}

return {
	Message = {} :: Message,
	ChatConfig = {} :: ChatConfig,
}
