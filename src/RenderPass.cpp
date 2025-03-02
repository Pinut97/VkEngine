#include "RenderPass.h"
#include "VulkanSwapChain.h"
#include "VulkanDevice.h"
#include "DepthBuffer.h"

RenderPass::RenderPass(const class VulkanSwapChain& swapChain, const class DepthBuffer& depthBuffer) : 
	swapChain_(swapChain),
	depthBuffer_(depthBuffer)
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format			= swapChain.scPublicVariables.format;
	colorAttachment.samples			= VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp			= VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout		= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription depthAttachment;
	depthAttachment.format			= depthBuffer.Format();
	depthAttachment.samples			= VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp			= VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout		= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	depthAttachment.flags			= 0;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment	= 0;
	colorAttachmentRef.layout		= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment	= 1;
	depthAttachmentRef.layout		= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;


	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint		= VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount	= 1;
	subpass.pColorAttachments		= &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

	/*
	VkSubpassDependency dependency{};
	dependency.srcSubpass		= VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass		= 0;
	dependency.srcStageMask		= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask	= 0;
	dependency.dstStageMask		= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask	= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	*/
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType			= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount	= static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments		= attachments.data();
	renderPassInfo.subpassCount		= 1;
	renderPassInfo.pSubpasses		= &subpass;
	renderPassInfo.dependencyCount	= 0;
	renderPassInfo.pDependencies = nullptr;// &dependency;

	if (vkCreateRenderPass(swapChain.Device()->Handle(), &renderPassInfo, nullptr, &renderPass_) != VK_SUCCESS)
		throw std::runtime_error("Failed to create Render pass!");
}

RenderPass::~RenderPass()
{
	if (renderPass_ != nullptr) {
		vkDestroyRenderPass(swapChain_.Device()->Handle(), renderPass_, nullptr);
		renderPass_ = nullptr;
	}
}