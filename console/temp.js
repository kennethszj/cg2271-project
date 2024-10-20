// App.js
import React, { useState } from "react";
import {
	ChakraProvider,
	Tabs,
	TabList,
	TabPanels,
	Tab,
	TabPanel,
	Button,
	Box,
	VStack,
	Heading,
	Icon,
} from "@chakra-ui/react";
import {
	FaStopCircle,
	FaLongArrowAltUp,
	FaLongArrowAltDown,
} from "react-icons/fa";
import { GiClockwiseRotation, GiAnticlockwiseRotation } from "react-icons/gi";
import { useToast } from "@chakra-ui/react";

function App() {
	const [isLoading, setIsLoading] = useState(false);
	const toast = useToast();

	const baseUrl = `http://${window.location.hostname}:3000/`;
	const sendCommand = (path) => {
		setIsLoading(true);
		fetch(`${baseUrl}${path}`, {
			method: "GET",
		})
			// .then((resp) => resp.json())
			.then(() =>
				toast({
					title: "Command sent!",
					status: "success",
					duration: 500,
					isClosable: true,
				})
			)
			.catch((err) => {
				toast({
					title: "Error sending command",
					description: err.message,
					status: "error",
					duration: 500,
					isClosable: true,
				});
			})
			.finally(() => {
				setIsLoading(false);
			});
	};
	return (
		<ChakraProvider>
			<Box p={4} maxWidth="600px" mx="auto">
				{" "}
				{/* This box centers the content and limits the width */}
				{/* Title for the page */}
				<Heading textAlign="center" mb={8} fontSize="2xl" color="teal.500">
					CG2271 Console
				</Heading>
				<Tabs variant="enclosed">
					<Box overflowX="auto" overflowY="hidden">
						{" "}
						{/* Ensures horizontal scrolling for tabs if they overflow */}
						<TabList>
							<Tab
								whiteSpace="nowrap"
								_selected={{ color: "white", bg: "teal.500" }}
								fontWeight="bold"
							>
								Default
							</Tab>
							<Tab
								whiteSpace="nowrap"
								_selected={{ color: "white", bg: "teal.500" }}
								fontWeight="bold"
							>
								Terminal
							</Tab>
							<Tab
								whiteSpace="nowrap"
								_selected={{ color: "white", bg: "teal.500" }}
								fontWeight="bold"
							>
								Led Status
							</Tab>
							<Tab
								whiteSpace="nowrap"
								_selected={{ color: "white", bg: "teal.500" }}
								fontWeight="bold"
							>
								Extras
							</Tab>
						</TabList>
					</Box>

					<TabPanels>
						<TabPanel px={0}>
							<VStack spacing={4}>
								<Button
									width="100%"
									p={6}
									isLoading={isLoading}
									loadingText="Sending Command"
									onClick={() => sendCommand("stop")}
									leftIcon={<Icon as={FaStopCircle} />}
								>
									Stop movement
								</Button>
								<Button
									width="100%"
									p={6}
									isLoading={isLoading}
									loadingText="Sending Command"
									onClick={() => sendCommand("forward")}
									leftIcon={<Icon as={FaLongArrowAltUp} />}
								>
									Move forward
								</Button>
								<Button
									width="100%"
									p={6}
									isLoading={isLoading}
									loadingText="Sending Command"
									onClick={() => sendCommand("backward")}
									leftIcon={<Icon as={FaLongArrowAltDown} />}
								>
									Move backward
								</Button>
								<Button
									width="100%"
									p={6}
									isLoading={isLoading}
									loadingText="Sending Command"
									onClick={() => sendCommand("clockwise")}
									leftIcon={<Icon as={GiClockwiseRotation} />}
								>
									Rotate clockwise
								</Button>
								<Button
									width="100%"
									p={6}
									isLoading={isLoading}
									loadingText="Sending Command"
									onClick={() => sendCommand("anticlockwise")}
									leftIcon={<Icon as={GiAnticlockwiseRotation} />}
								>
									Rotate anticlockwise
								</Button>
							</VStack>
						</TabPanel>
						<TabPanel px={0}>
							<VStack spacing={4}>
								<Button
									width="100%"
									p={6}
									isLoading={isLoading}
									loadingText="Sending Command"
									onClick={() => sendCommand("playtone")}
								>
									Play sound
								</Button>
							</VStack>
						</TabPanel>
						<TabPanel px={0}>
							<VStack spacing={4}>
								{/* <Button
									width="100%"
									p={6}
									isLoading={isLoading}
									loadingText="Sending Command"
									onClick={() => sendCommand("")}
								>
									Button 1
								</Button> */}
							</VStack>
						</TabPanel>
						<TabPanel px={0}>
							<VStack spacing={4}>
								{/* <Button
									width="100%"
									p={6}
									isLoading={isLoading}
									loadingText="Sending Command"
									onClick={() => sendCommand("")}
								>
									Button
								</Button> */}
							</VStack>
						</TabPanel>
					</TabPanels>
				</Tabs>
			</Box>
		</ChakraProvider>
	);
}

export default App;
