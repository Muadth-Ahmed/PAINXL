// Text that will be "typed out"
const textForInfo =
  "PAINXL (Pointer-Advanced Impossible Nightmare eXecution Language) is an esoteric programming language created in 2025 by Muadth Ahmed. It was designed to be complex with minimal commands to challenge programmers and serve as the first step for anyone who wants to learn binary.\n\nFirst:\n\n  What are the commands in PAINXL, and what do you need to learn to use it?\n\nTo learn PAINXL, you will need to understand ASCII.\n\nCommands:\n\n\
01001001 (Move the pointer to the right)\n\n10101001 (Move the pointer to the left)\n\n00100101 (Increase the current cell by 1)\n\n10010101 (Decrease the current cell by 1)\n\n00000001 (Start a loop)\n\n11111101 (End a loop)\n\n00101101 (Set current cell to 0)\n\n00011101 (Get a character from the user)\n\n11100001 (Output an ASCII character)\n\nHere is a simple code in PAINXL:\n\n\
00100101001001010010010100100101001001010010010\n100100101001001010010010100100101\n0000000101001001001001010010010100100101001001010010010\n10010010100100101101010011001010111111101\n01001001001001010010010111100001\n\n\
You can use it on the web by following this link: https://painxl-lang.org/interpreter/";

// Get the output element
const element = document.getElementById("infoP");
let infoIndex = 0;

// Regex to detect URLs
const urlRegex = /(https?:\/\/[^\s]+)/;

// Typewriter function
function typeWriterInfo() {
  generate.style.display = "none"; // hide the button

  if (infoIndex < textForInfo.length) {
    element.style.display = "block";

    // Get the remaining text
    const remainingText = textForInfo.substring(infoIndex);
    const match = remainingText.match(urlRegex);

    if (match && match.index === 0) {
      // Create clickable link
      const a = document.createElement("a");
      a.href = match[0];
      a.target = "_blank"; // open in new tab
      a.textContent = match[0];
      a.style.color = "#306998"; // optional styling
      a.style.textDecoration = "underline";
      element.appendChild(a);

      infoIndex += match[0].length; // skip over the URL
    } else {
      // Add normal character
      element.appendChild(
        document.createTextNode(textForInfo.charAt(infoIndex))
      );
      infoIndex++;
    }

    setTimeout(typeWriterInfo, Math.random() * 50);
  }
}

// Title typing
let textToTitle = "PAINXL Official Website.";
let titleIndex = 0;
function typeWriterTitle() {
  if (titleIndex < textToTitle.length) {
    document.title += textToTitle.charAt(titleIndex);
    titleIndex++;
    setTimeout(typeWriterTitle, 50);
  }
}

// Button to start typing
const generate = document.getElementById("generate");
generate.addEventListener("click", typeWriterInfo);

// Start title typing immediately
typeWriterTitle();

// Download links
document.getElementById("linux").addEventListener("click", function () {
  const link = document.createElement("a");
  link.href = "Linux.zip";
  link.download = "Linux.zip";
  document.body.appendChild(link);
  link.click();
  document.body.removeChild(link);
});
document.getElementById("windows").addEventListener("click", function () {
  const link = document.createElement("a");
  link.href = "Windows.zip";
  link.download = "Windows.zip";
  document.body.appendChild(link);
  link.click();
  document.body.removeChild(link);
});
