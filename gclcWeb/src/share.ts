import {
  compressToEncodedURIComponent,
  decompressFromEncodedURIComponent,
} from "lz-string";
import { getCode } from "./editor";
import { showPopup } from "./popup";

const getCodeFromUrl = (): string => {
  const searchParams = new URLSearchParams(window.location.search);

  if (searchParams.has("code")) {
    const compressedCode = searchParams.get("code") as string;
    history.pushState({}, "", baseURL());

    const code = decompressFromEncodedURIComponent(compressedCode);
    return code;
  }

  return "";
};

const formatUrl = (code: string): string => {
  const compressed = compressToEncodedURIComponent(code);
  return baseURL() + "?code=" + compressed;
};

const baseURL = () => {
  const urlObj = new URL(window.location.href);
  urlObj.search = "";
  return urlObj.toString();
};

const shareHandler = () => {
  const url = formatUrl(getCode());
  if (url.length < 2000) {
    const shareA = document.getElementById("shareUrl")! as HTMLAnchorElement;
    shareA.textContent = url;
    shareA.href = url;
    showPopup("sharePopup")();
  } else {
    showPopup("codeTooLongToSharePopup")();
  }
};

const setShareUI = () => {
  document
    .getElementById("shareButton")
    ?.addEventListener("click", shareHandler);
};

export { getCodeFromUrl, setShareUI };
