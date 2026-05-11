Create migration:
dotnet ef migrations add InitialCreate \
--project DataAccess \
--startup-project Api

Apply migration to database:
dotnet ef database update \
--project DataAccess \
--startup-project Api