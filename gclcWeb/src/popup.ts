const closePopups = () => {
  const backdrop = document.getElementById("backdrop")!;
  backdrop.classList.remove("backdropBlur");
  setTimeout(() => {
    backdrop.style.display = "none";
  }, 200);

  const popups = document.getElementsByClassName("popup");
  for (const popup of popups) {
    (popup as HTMLElement).style.display = "none";
  }
};

const showPopup = (id: string) => () => {
  const backdrop = document.getElementById("backdrop")!;
  backdrop.style.display = "block";
  setTimeout(() => {
    backdrop.classList.add("backdropBlur");
  }, 0);

  const box = document.getElementById(id)!;
  if (box !== null) {
    box.style.display = "block";
  }
};

const setPopupUI = () => {
  document.getElementById("backdrop")?.addEventListener("click", closePopups);

  const popups = document.getElementsByClassName("close");
  for (const popup of popups) {
    (popup as HTMLElement).addEventListener("click", closePopups);
  }

  document.onkeydown = (e) => {
    if (e.key == "Escape") {
      closePopups();
    }
  };
};

export { closePopups, showPopup, setPopupUI };
