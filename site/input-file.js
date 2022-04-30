export default (input, onFileLoad) => {
    const handleFile = (file) => {
        if (!file) return;

        const reader = new FileReader();
        reader.onload = () => {
            onFileLoad(reader.result);
        };
        reader.readAsArrayBuffer(file);
    };

    input.addEventListener(
        "change",
        function () {
            handleFile(this.files[0]);
        },
        false
    );

    handleFile(input.files[0]);
};
