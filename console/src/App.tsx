// App.js
import {
	ChakraProvider,
	Box,
	Button,
	IconButton,
	VStack,
	HStack,
} from "@chakra-ui/react";
import {
	ArrowUpIcon,
	ArrowDownIcon,
	ArrowBackIcon,
	ArrowForwardIcon,
} from "@chakra-ui/icons";
import { FaUndoAlt, FaRedoAlt } from "react-icons/fa";

import { useToast } from "@chakra-ui/react";

function App() {
	const toast = useToast();

	const baseUrl = `http://192.168.43.201`;
	const sendCommand = (cmd: string) => {
		fetch(`${baseUrl}/${cmd}`, {
			method: "GET",
			mode: "no-cors",
		}).catch((err) => {
			toast({
				title: "Error sending command",
				description: err.message,
				status: "error",
				duration: 100,
				isClosable: true,
			});
		});
	};

	return (
		<ChakraProvider>
			<Box
				display="flex"
				alignItems="center"
				justifyContent="center"
				minH="100vh"
				bg="gray.50"
			>
				<VStack spacing={6}>
					{/* Forward Button with Secondary Half-Button */}
					<Box position="relative">
						<Button
							size="lg"
							colorScheme="blue"
							height="80px"
							width="80px"
							aria-label="Forward"
							onMouseDown={() => sendCommand("forward")}
							onMouseUp={() => sendCommand("stop")}
							onTouchStart={() => sendCommand("forward")}
							onTouchEnd={() => sendCommand("forward")}
						>
							<ArrowUpIcon boxSize={6} />
						</Button>
						<IconButton
							icon={<ArrowUpIcon />}
							aria-label="Forward Half"
							colorScheme="blue"
							size="sm"
							position="absolute"
							top="-40px"
							left="20px"
							onMouseDown={() => sendCommand("halfspeedforward")}
							onMouseUp={() => sendCommand("stop")}
							onTouchStart={() => sendCommand("halfspeedforward")}
							onTouchEnd={() => sendCommand("stop")}
						/>
					</Box>

					{/* Row with Curve Left and Curve Right Buttons, each with a nearby rotation button */}
					<HStack spacing={4} justify="center">
						{/* Curve Left Button with Anticlockwise Rotation */}
						<Box position="relative">
							<Button
								size="lg"
								colorScheme="orange"
								height="80px"
								width="80px"
								aria-label="Curve Left"
								onMouseDown={() => sendCommand("curveanticlockwise")}
								onMouseUp={() => sendCommand("stop")}
								onTouchStart={() => sendCommand("curveanticlockwise")}
								onTouchEnd={() => sendCommand("stop")}
							>
								<ArrowBackIcon boxSize={6} />
							</Button>
							<IconButton
								icon={<FaUndoAlt />}
								aria-label="Anticlockwise Rotation"
								colorScheme="yellow"
								size="sm"
								position="absolute"
								bottom="-10px"
								left="-40px"
								onMouseDown={() => sendCommand("anticlockwise")}
								onMouseUp={() => sendCommand("stop")}
								onTouchStart={() => sendCommand("anticlockwise")}
								onTouchEnd={() => sendCommand("stop")}
							/>
						</Box>

						{/* Curve Right Button with Clockwise Rotation */}
						<Box position="relative">
							<Button
								size="lg"
								colorScheme="green"
								height="80px"
								width="80px"
								aria-label="Curve Right"
								onMouseDown={() => sendCommand("curveclockwise")}
								onMouseUp={() => sendCommand("stop")}
								onTouchStart={() => sendCommand("curveclockwise")}
								onTouchEnd={() => sendCommand("stop")}
							>
								<ArrowForwardIcon boxSize={6} />
							</Button>
							<IconButton
								icon={<FaRedoAlt />}
								aria-label="Clockwise Rotation"
								colorScheme="purple"
								size="sm"
								position="absolute"
								bottom="-10px"
								right="-40px"
								onMouseDown={() => sendCommand("clockwise")}
								onMouseUp={() => sendCommand("stop")}
								onTouchStart={() => sendCommand("clockwise")}
								onTouchEnd={() => sendCommand("stop")}
							/>
						</Box>
					</HStack>

					{/* Backward Button with Secondary Half-Button */}
					<Box position="relative">
						<Button
							size="lg"
							colorScheme="red"
							height="80px"
							width="80px"
							aria-label="Backward"
							onMouseDown={() => sendCommand("backward")}
							onMouseUp={() => sendCommand("stop")}
							onTouchStart={() => sendCommand("backward")}
							onTouchEnd={() => sendCommand("stop")}
						>
							<ArrowDownIcon boxSize={6} />
						</Button>
						<IconButton
							icon={<ArrowDownIcon />}
							aria-label="Backward Half"
							colorScheme="red"
							size="sm"
							position="absolute"
							bottom="-40px"
							left="20px"
							onMouseDown={() => sendCommand("halfspeedbackward")}
							onMouseUp={() => sendCommand("stop")}
							onTouchStart={() => sendCommand("halfspeedbackward")}
							onTouchEnd={() => sendCommand("stop")}
						/>
					</Box>
					<Button
						mt={20}
						size="lg"
						colorScheme="yellow"
						height="80px"
						width="100%"
						aria-label="Complete"
						onMouseDown={() => sendCommand("playtone")}
						onTouchStart={() => sendCommand("playtone")}
					>
						Complete
					</Button>
				</VStack>
			</Box>
		</ChakraProvider>
	);
}

export default App;
