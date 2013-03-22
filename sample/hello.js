(function() {
  dust.register("sample/hello", body_0);

  function body_0(chk, ctx) {
    return chk.write("Hey ").reference(ctx.get("name"), ctx, "h").write(". ").reference(ctx.get("app"), ctx, "h").write(" just rendered this!");
  }
  return body_0;
})();